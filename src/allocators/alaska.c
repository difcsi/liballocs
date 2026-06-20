/*
 * Alaska backing-heap allocator.
 *
 * Lets liballocs answer metadata queries (base / size / type) for objects that
 * live in Alaska's handle-based heap. liballocs stays *handle-agnostic*: this
 * allocator only ever sees raw backing pointers (the kind stackscan's
 * translate-then-index already hands to liballocs). It owns the one contiguous
 * region Alaska mmaps for its sized heap and resolves:
 *
 *   - base/size from Alaska's page table (alaska_object_base / alaska_object_size),
 *   - type   from the recorded allocsite of the object via the normal liballocs
 *            allocsite -> uniqtype machinery (__liballocs_find_allocsite_entry_at).
 *
 * The Alaska helpers are weak-imported so liballocs still links/runs when no
 * Alaska runtime is present (queries then simply never route here).
 *
 * Registration is lazy: liballocs deliberately leaves Alaska's heap mmap
 * un-indexed, so the first query for a backing address misses and reaches
 * __alaska_allocator_notify_unindexed_address (below), which claims the
 * containing Alaska page as a bigalloc owned by this allocator. Doing this from
 * liballocs' own query path -- not Alaska's runtime -- avoids the re-entrancy the
 * mmap allocator guards against.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <dlfcn.h>
#include "liballocs_private.h"

/* Alaska runtime exports (rt/liballocs_export.cpp, rt/halloc.cpp). Weak: absent
 * when no Alaska runtime is loaded. */
extern void          *alaska_object_base(void *backing) __attribute__((weak));
extern unsigned long  alaska_object_size(void *backing)  __attribute__((weak));
extern int            alaska_heap_page_extent(void *backing, void **out_base,
                          unsigned long *out_size) __attribute__((weak));
extern void          *alaska_translate(void *ptr) __attribute__((weak));
extern void           alaska_hfree_now(void *ptr) __attribute__((weak));

#ifdef LIFETIME_POLICIES
/* Alaska reserves exactly sizeof(struct insert) trailing bytes per sized object
 * (ALASKA_LIBALLOCS_INSERT_RESERVE in the Alaska build). Keep the two in lockstep. */
_Static_assert(sizeof(struct insert) == 8,
	"Alaska's ALASKA_LIBALLOCS_INSERT_RESERVE assumes an 8-byte struct insert");
#endif

/* ------------------------------------------------------------------------- */
/* Per-object side table.                                                    */
/*                                                                           */
/* Alaska objects carry no liballocs trailer, so we keep per-object metadata  */
/* in a side table keyed by object base: the allocation site (caller PC, for   */
/* typing) and the caller's *requested* size (so get_info reports the exact    */
/* size, not Alaska's rounded-up size class -- giving exact heap-array         */
/* lengths, mirroring the generic-malloc padding mechanism). Populated/cleared */
/* from the halloc/hfree path (__liballocs_notify_alaska_*).                   */
/* ------------------------------------------------------------------------- */

#define ALASKA_SITE_TABLE_BITS 16
#define ALASKA_SITE_TABLE_SIZE (1u << ALASKA_SITE_TABLE_BITS)
#define ALASKA_SITE_TABLE_MASK (ALASKA_SITE_TABLE_SIZE - 1)

struct alaska_site_entry
{
	void *base;        /* object base; NULL == empty slot */
	const void *site;  /* recorded allocation site (caller PC) */
	unsigned long size; /* caller-requested size in bytes (0 == unknown) */
};

static struct alaska_site_entry alaska_site_table[ALASKA_SITE_TABLE_SIZE];
static pthread_mutex_t alaska_site_lock = PTHREAD_MUTEX_INITIALIZER;

static unsigned alaska_site_hash(void *base)
{
	/* Object bases are page-table-aligned within a size class; mix the high
	 * bits down. Fibonacci hashing keeps the linear-probe clusters short. */
	uintptr_t x = (uintptr_t) base;
	x ^= x >> 21;
	x *= 0x9E3779B97F4A7C15ull;
	return (unsigned) (x >> (64 - ALASKA_SITE_TABLE_BITS)) & ALASKA_SITE_TABLE_MASK;
}

void __alaska_allocator_record_object(void *base, const void *site, unsigned long size)
{
	if (!base) return;
	pthread_mutex_lock(&alaska_site_lock);
	unsigned h = alaska_site_hash(base);
	for (unsigned i = 0; i < ALASKA_SITE_TABLE_SIZE; ++i)
	{
		struct alaska_site_entry *e = &alaska_site_table[(h + i) & ALASKA_SITE_TABLE_MASK];
		if (e->base == NULL || e->base == base)
		{
			e->base = base;
			e->site = site;
			e->size = size;
			pthread_mutex_unlock(&alaska_site_lock);
			return;
		}
	}
	/* Table full: drop the record. get_info degrades to type=NULL and the
	 * (rounded-up) size class. */
	pthread_mutex_unlock(&alaska_site_lock);
}

void __alaska_allocator_forget_object(void *base)
{
	if (!base) return;
	pthread_mutex_lock(&alaska_site_lock);
	unsigned h = alaska_site_hash(base);
	for (unsigned i = 0; i < ALASKA_SITE_TABLE_SIZE; ++i)
	{
		struct alaska_site_entry *e = &alaska_site_table[(h + i) & ALASKA_SITE_TABLE_MASK];
		if (e->base == NULL) break;       /* end of probe chain */
		if (e->base == base) { e->base = NULL; e->site = NULL; e->size = 0; break; }
	}
	pthread_mutex_unlock(&alaska_site_lock);
}

/* Look up base; on hit fill *out_site/*out_size and return 1, else return 0. */
static _Bool alaska_lookup_object(void *base, const void **out_site, unsigned long *out_size)
{
	_Bool found = 0;
	pthread_mutex_lock(&alaska_site_lock);
	unsigned h = alaska_site_hash(base);
	for (unsigned i = 0; i < ALASKA_SITE_TABLE_SIZE; ++i)
	{
		struct alaska_site_entry *e = &alaska_site_table[(h + i) & ALASKA_SITE_TABLE_MASK];
		if (e->base == NULL) break;
		if (e->base == base)
		{
			if (out_site) *out_site = e->site;
			if (out_size) *out_size = e->size;
			found = 1;
			break;
		}
	}
	pthread_mutex_unlock(&alaska_site_lock);
	return found;
}

/* ------------------------------------------------------------------------- */
/* Allocation-site capture, driven by the Alaska runtime.                    */
/*                                                                           */
/* allocscc instruments each halloc call site to stash the caller PC in       */
/* liballocs' __current_allocsite TLS before calling halloc -- but Alaska      */
/* allocates from its own heap, so the libc-malloc hook that would normally    */
/* consume __current_allocsite never fires. Instead, Alaska's halloc/hfree     */
/* call these hooks (weakly, so it builds without liballocs); we read the      */
/* pending allocsite and bind it to the object's backing base. get_info then   */
/* resolves the type through the ordinary allocsite -> uniqtype table.         */
/* ------------------------------------------------------------------------- */

#ifdef LIFETIME_POLICIES
/* Pointer to the object's trailing liballocs insert (which carries its
 * lifetime-policy mask), or NULL if Alaska/this object can't host one. Alaska
 * reserves sizeof(struct insert) bytes at the end of every sized slot and folds
 * it into size_of(), so insert_for_chunk lands in that reserved tail -- never on
 * user data -- and the tail is copied with the object when the GC relocates it. */
static inline struct insert *alaska_lifetime_insert(void *base)
{
	if (!&alaska_object_size || !base) return NULL;
	if (alaska_object_size(base) < sizeof(struct insert)) return NULL;
	return insert_for_chunk(base, alaska_object_size);
}

/* Alaska's native refcount+stackscan is the GC; liballocs need only model it as a
 * lifetime policy so the two-policy accounting (manual vs GC) holds. Register one
 * no-op GC policy lazily and reuse its id as the per-object "GC owns this" bit.
 * The callbacks do nothing -- the native GC performs the real reclamation. */
static void alaska_gc_noop_cb(const void *target, const void **from) { (void)target; (void)from; }
static int alaska_gc_policy_id;  /* 0 until registered (0 is the MANUAL id, never a GC id) */
static int alaska_ensure_gc_policy(void)
{
	int id = __atomic_load_n(&alaska_gc_policy_id, __ATOMIC_ACQUIRE);
	if (id) return id;
	id = __liballocs_register_gc_policy(alaska_gc_noop_cb, alaska_gc_noop_cb);
	if (id <= 0) return 0;
	int expected = 0;
	if (!__atomic_compare_exchange_n(&alaska_gc_policy_id, &expected, id, 0,
			__ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE))
		return expected;  /* lost the race; another thread registered (ours leaks a slot) */
	return id;
}
#endif

void __liballocs_notify_alaska_alloc(void *backing_base, unsigned long requested_size)
{
	/* __current_allocsite is the pending halloc site (may be NULL for an
	 * un-instrumented caller); record the object either way so the exact
	 * requested size is still available even when the type isn't. */
	if (!backing_base) return;
	__alaska_allocator_record_object(backing_base, __current_allocsite, requested_size);
#ifdef LIFETIME_POLICIES
	/* Seed the trailing insert with the GC lifetime policy only; the manual policy
	 * is attached on demand through the liballocs API (__liballocs_attach_lifetime_
	 * policy / the ss_attach_manual_policy wrapper). The GC bit keeps the mask
	 * non-empty so a later manual detach (the hijacked hfree) never trips liballocs'
	 * free-on-empty path; the native Alaska GC is the sole performer of the free. */
	int gcid = alaska_ensure_gc_policy();
	struct insert *ins = gcid ? alaska_lifetime_insert(backing_base) : NULL;
	if (ins)
	{
		memset(ins, 0, sizeof *ins);
		ins->common.lifetime_policies = LIFETIME_POLICY_FLAG(gcid);
	}
#endif
}

void __liballocs_notify_alaska_free(void *backing_base)
{
	__alaska_allocator_forget_object(backing_base);
}

/* ------------------------------------------------------------------------- */
/* Re-entrancy guard.                                                        */
/*                                                                           */
/* Querying Alaska for an object's base/size can make Alaska allocate (grow   */
/* its page table, lazily create a thread cache, ...). Those allocations are  */
/* hooked by liballocs and trigger fresh metadata queries which, for an       */
/* un-indexed Alaska address, would re-enter this allocator -- an unbounded   */
/* recursion (stack overflow). While we are talking to Alaska we therefore    */
/* short-circuit any nested entry: the nested query simply reports the address */
/* as not-ours, exactly like liballocs' mmap allocator skips libalaska maps.  */
/* ------------------------------------------------------------------------- */
static __thread int in_alaska_allocator;

/* ------------------------------------------------------------------------- */
/* The allocator vtable.                                                     */
/* ------------------------------------------------------------------------- */

static liballocs_err_t get_info(void *obj, struct big_allocation *maybe_bigalloc,
	struct uniqtype **out_type, void **out_base,
	unsigned long *out_size, const void **out_site)
{
	if (!&alaska_object_base || !&alaska_object_size || in_alaska_allocator)
	{
		return &__liballocs_err_unindexed_heap_object;
	}
	in_alaska_allocator = 1;

	liballocs_err_t err = NULL;
	void *base = alaska_object_base(obj);
	if (!base)
	{
		/* Not in Alaska's sized heap (e.g. a huge object in a separate mmap). */
		++__liballocs_aborted_unindexed_heap;
		err = &__liballocs_err_unindexed_heap_object;
		goto out;
	}
	/* Prefer the caller's recorded *requested* size (exact array lengths); fall
	 * back to Alaska's rounded-up size class for un-recorded objects. */
	const void *site = NULL;
	unsigned long rec_size = 0;
	_Bool recorded = alaska_lookup_object(base, &site, &rec_size);
	unsigned long size = (recorded && rec_size) ? rec_size : alaska_object_size(base);

	if (out_base) *out_base = base;
	if (out_size) *out_size = size;
	if (out_site) *out_site = site;

	if (out_type)
	{
		struct uniqtype *t = NULL;
		if (site)
		{
			struct allocsite_entry *entry = __liballocs_find_allocsite_entry_at(site);
			t = entry ? entry->uniqtype : NULL;
			if (!t && !__liballocs_addrlist_contains(
					&__liballocs_unrecognised_heap_alloc_sites, (void*) site))
			{
				__liballocs_addrlist_add(&__liballocs_unrecognised_heap_alloc_sites,
					(void*) site);
			}
		}
		*out_type = t;
		if (!t)
		{
			++__liballocs_aborted_unrecognised_allocsite;
			err = &__liballocs_err_unrecognised_alloc_site;
		}
	}
out:
	in_alaska_allocator = 0;
	return err;
}

static unsigned long get_size(void *obj)
{
	if (!&alaska_object_base || !&alaska_object_size) return 0;
	void *base = alaska_object_base(obj);
	return base ? alaska_object_size(base) : 0;
}

struct allocator __alaska_heap_allocator = {
	.name = "alaska",
	.min_alignment = 16, /* Alaska aligns allocations to 16 bytes (AlignedSize). */
	/* NOT cacheable: Alaska's GC compacts/relocates objects, so a cached
	 * base/size would go stale after a collection. */
	.is_cacheable = 0,
	.get_info = get_info,
	.get_size = get_size,
};

/* ------------------------------------------------------------------------- */
/* Lazy region claim, driven by liballocs' own query path.                   */
/*                                                                           */
/* liballocs deliberately does NOT index Alaska's heap mmap (its mmap         */
/* allocator skips libalaska-made mappings to avoid an init-time re-entrancy  */
/* loop), so there is no bigalloc covering backing pointers until we make one.*/
/* When a query for an un-indexed address arrives, __liballocs_get_alloc_info */
/* calls __liballocs_notify_unindexed_address, which dispatches here. We claim */
/* the containing Alaska page (2 MiB) as a bigalloc owned by this allocator    */
/* and return 1; the query then retries and routes to get_info. Doing this at */
/* query time -- not from Alaska's runtime context -- keeps us clear of the    */
/* re-entrancy the mmap allocator guards against. Page-granular claims also    */
/* stay well under liballocs' max-bigalloc size (~4 GiB < Alaska's 8 GiB heap).*/
/* ------------------------------------------------------------------------- */

_Bool __alaska_allocator_notify_unindexed_address(const void *ptr)
{
	if (!&alaska_heap_page_extent) return 0;     /* no Alaska runtime loaded */
	/* Don't recurse: a nested query raised while we are already talking to
	 * Alaska (e.g. Alaska allocating page-table nodes) -- or while we create the
	 * bigalloc below -- is not ours to claim. Hold the guard across the whole
	 * handler so any such nested entry short-circuits. */
	if (in_alaska_allocator) return 0;
	in_alaska_allocator = 1;

	_Bool ret = 0;
	void *page_base = NULL;
	unsigned long page_size = 0;
	if (alaska_heap_page_extent((void *) ptr, &page_base, &page_size))
	{
		/* Already claimed (e.g. by an earlier query into the same page)? Then the
		 * leaf lookup simply hadn't been retried yet; report success. */
		if (__lookup_deepest_bigalloc(ptr)) ret = 1;
		else ret = (NULL != __liballocs_new_bigalloc(
			page_base,
			(size_t) page_size,
			NULL, /* allocator_private */
			NULL, /* allocator_private_free */
			NULL, /* maybe_parent -- top-level; Alaska pages are page-aligned */
			&__alaska_heap_allocator));
	}

	in_alaska_allocator = 0;
	return ret;
}

#ifdef LIFETIME_POLICIES
/* ------------------------------------------------------------------------- */
/* Manual lifetime policy: hijacked hfree + the GC's pin query.              */
/* ------------------------------------------------------------------------- */

/* Is the manual lifetime policy attached to the Alaska object based at `base`?
 * Read straight from the object's own trailing insert -- no lock and no Alaska
 * heap query -- so the in-barrier GC reclaim (rt/refcount.cpp) can call it with
 * the world stopped. Weak-imported there, so Alaska built without liballocs just
 * reclaims as before. */
int __liballocs_alaska_manual_pinned(void *base)
{
	struct insert *ins = alaska_lifetime_insert(base);
	if (!ins) return 0;
	return (ins->common.lifetime_policies & MANUAL_DEALLOCATION_FLAG) != 0;
}

/* Hijack hfree -- Alaska's manual-free entry point -- the way liballocs hijacks
 * free (Bertholon & Kell). Instead of freeing, DETACH the manual policy: the
 * object stays alive (its GC policy bit remains set, so the mask never empties
 * and liballocs' detach never frees here), and the native Alaska GC reclaims it
 * once it is also at refcount 0 and stack-unreachable. Objects we don't manage
 * (huge objects, or no Alaska runtime) get a genuine free. This strong symbol
 * preempts Alaska's own hfree because liballocs is earlier in LD_PRELOAD. */
void hfree(void *ptr)
{
	if (!ptr) return;
	if (&alaska_translate && &alaska_object_base)
	{
		void *base = alaska_translate(ptr);
		if (base && alaska_object_base(base))
		{
			__liballocs_detach_lifetime_policy(MANUAL_DEALLOCATION_POLICY, base);
			return;
		}
	}
	/* Not a lifetime-managed sized Alaska object. */
	if (&alaska_hfree_now) { alaska_hfree_now(ptr); return; }
	/* No Alaska runtime at all: chain transparently to the next hfree. */
	static void (*next_hfree)(void *);
	if (!next_hfree) next_hfree = (void (*)(void *)) dlsym(RTLD_NEXT, "hfree");
	if (next_hfree && next_hfree != hfree) next_hfree(ptr);
}
#endif
