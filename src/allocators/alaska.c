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
#include "relf.h"   /* fake_dlsym: lock-free symbol lookup (see alaska_resolve_syms) */

/* Alaska runtime exports (rt/liballocs_export.cpp, rt/halloc.cpp). Weak: absent
 * when no Alaska runtime is loaded. */
extern void          *alaska_object_base(void *backing) __attribute__((weak));
extern unsigned long  alaska_object_size(void *backing)  __attribute__((weak));
extern int            alaska_heap_page_extent(void *backing, void **out_base,
                          unsigned long *out_size) __attribute__((weak));
extern void          *alaska_translate(void *ptr) __attribute__((weak));
extern void           alaska_hfree_now(void *ptr) __attribute__((weak));
extern void          *alaska_heap_start(void) __attribute__((weak));
extern unsigned long  alaska_heap_size(void)  __attribute__((weak));
extern void			 *alaska_handle_for(void *ptr) __attribute__((weak));
/* liballocs is LD_PRELOADed, so it is relocated at process start -- BEFORE libalaska
 * is dlopened (as a DT_NEEDED of the first Alaska-transformed .so). Its weak
 * undefined references to the alaska_* symbols above therefore bind to 0 at load and
 * are never fixed up once libalaska later enters the (global) namespace: every
 * `&alaska_object_base` etc. stays NULL and the whole allocator silently disables
 * itself (queries for Alaska backing pointers all abort as "unknown storage"). Fix:
 * resolve the symbols lazily through dlsym(RTLD_DEFAULT) on first use -- by the time
 * any backing pointer is queried, libalaska is loaded and in the global scope. If a
 * weak ref *did* resolve at load (e.g. a non-preload/static link), use it directly. */
static void          *(*pf_alaska_object_base)(void *);
static unsigned long  (*pf_alaska_object_size)(void *);
static int            (*pf_alaska_heap_page_extent)(void *, void **, unsigned long *);
static void          *(*pf_alaska_heap_start)(void);
static unsigned long  (*pf_alaska_heap_size)(void);
static void          *(*pf_alaska_translate)(void *);
static void           (*pf_alaska_hfree_now)(void *);
static void           *(*pf_alaska_handle_for)(void *);
/* alaska::is_initialized() -- reads a plain volatile bool, so it is safe to call at
 * any moment (even while libalaska is still loading). Called from HERE (the preload)
 * through a fake_dlsym-resolved pointer -- a direct call, NOT libalaska's lazy PLT --
 * so it can't crash mid-load the way a cross-DSO PLT call inside libalaska would.
 * C++ mangled name of `bool alaska::is_initialized(void)`. */
static _Bool          (*pf_alaska_is_initialized)(void);
static _Bool alaska_syms_resolved;

/* Resolve a symbol in the global scope WITHOUT taking the dynamic-linker lock.
 * dlsym() would: liballocs indexes objects as they are loaded, so a metadata query
 * (which reaches this allocator) can fire while ld.so already holds its load lock --
 * re-entering it via dlsym deadlocks/crashes. librunt's fake_dlsym walks the link
 * map directly (and understands ifuncs); it returns (void*)-1 when not found. */
static void *alaska_lockfree_sym(const char *name)
{
	void *s = fake_dlsym(RTLD_DEFAULT, (char *) name);
	return (s == (void *) -1) ? NULL : s;
}

static void alaska_resolve_syms(void)
{
	if (alaska_syms_resolved) return;
	if (!pf_alaska_object_base)      pf_alaska_object_base      = &alaska_object_base      ? alaska_object_base      : alaska_lockfree_sym("alaska_object_base");
	if (!pf_alaska_object_size)      pf_alaska_object_size      = &alaska_object_size      ? alaska_object_size      : alaska_lockfree_sym("alaska_object_size");
	if (!pf_alaska_heap_page_extent) pf_alaska_heap_page_extent = &alaska_heap_page_extent ? alaska_heap_page_extent : alaska_lockfree_sym("alaska_heap_page_extent");
	if (!pf_alaska_heap_start)       pf_alaska_heap_start       = &alaska_heap_start       ? alaska_heap_start       : alaska_lockfree_sym("alaska_heap_start");
	if (!pf_alaska_heap_size)        pf_alaska_heap_size        = &alaska_heap_size        ? alaska_heap_size        : alaska_lockfree_sym("alaska_heap_size");
	if (!pf_alaska_translate)        pf_alaska_translate        = &alaska_translate        ? alaska_translate        : alaska_lockfree_sym("alaska_translate");
	if (!pf_alaska_hfree_now)        pf_alaska_hfree_now        = &alaska_hfree_now         ? alaska_hfree_now         : alaska_lockfree_sym("alaska_hfree_now");
	if (!pf_alaska_handle_for)        pf_alaska_handle_for        = &alaska_handle_for         ? alaska_handle_for         : alaska_lockfree_sym("alaska_handle_for");
	if (!pf_alaska_is_initialized)   pf_alaska_is_initialized   = alaska_lockfree_sym("_ZN6alaska14is_initializedEv"); // HACK
	/* Latch only once the symbols the query path needs are in hand. */
	if (pf_alaska_object_base && pf_alaska_object_size && pf_alaska_heap_page_extent
			&& pf_alaska_is_initialized)
		alaska_syms_resolved = 1;
}

/* Identify the Alaska runtime by its exported-symbol contract rather than its SO
 * name. Cache libalaska's link_map (the object defining the Alaska exports) and
 * test a PC by object identity. Robust to renaming/relocation; inert until the
 * runtime is mapped (its dynsym enters the link map the moment it is). Used by the
 * mmap allocator to skip indexing mappings the runtime makes for its own use. */
static struct link_map *alaska_runtime_lm;   /* NULL until identified */

_Bool __alaska_is_runtime_caller(const void *caller)
{
	if (!caller) return 0;
	if (!alaska_runtime_lm)
	{
		alaska_resolve_syms();
		/* Any resolved Alaska export anchors us inside libalaska's mapping. */
		void *anchor = (void *) pf_alaska_heap_start;
		if (!anchor) anchor = (void *) pf_alaska_object_base;
		if (anchor)
		{
			struct link_map *lm = get_highest_loaded_object_below(anchor);
			/* Require a named shared object: in a (non-default) static link the
			 * anchor lands in the main exe (empty l_name); don't claim that whole
			 * mapping as "Alaska". Stays inert in that case, like the old check. */
			if (lm && lm->l_name && lm->l_name[0]) alaska_runtime_lm = lm;
		}
		if (!alaska_runtime_lm) return 0;   /* not resolvable yet */
	}
	return get_highest_loaded_object_below(caller) == alaska_runtime_lm;
}


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
	alaska_resolve_syms();
	if (!pf_alaska_object_size || !base) return NULL;
	if (pf_alaska_object_size(base) < sizeof(struct insert)) return NULL;
	return insert_for_chunk(base, pf_alaska_object_size);
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

/* Is `base` a huge object? Huge objects live in their own mmap, outside Alaska's
 * one permanent sized-heap reservation [heap_start, heap_start+heap_size). */
static _Bool alaska_is_huge_base(void *base)
{
	alaska_resolve_syms();
	if (!pf_alaska_heap_start || !pf_alaska_heap_size) return 0;
	uintptr_t start = (uintptr_t) pf_alaska_heap_start();
	if (!start) return 0;
	uintptr_t b = (uintptr_t) base;
	return b < start || b >= start + pf_alaska_heap_size();
}

void __liballocs_notify_alaska_free(void *backing_base)
{
	__alaska_allocator_forget_object(backing_base);
	/* A huge object's backing mmap is handed back to the OS by Alaska right after
	 * this call (hfree -> huge_allocator.free -> munmap). The bigalloc we lazily
	 * claimed over that region (see __alaska_allocator_notify_unindexed_address)
	 * is NOT torn down by liballocs' munmap path -- that only deletes bigallocs
	 * owned by the mmap allocator, and this one is owned by __alaska_heap_allocator
	 * -- so it would dangle over an address the kernel may reissue to an unrelated
	 * mapping. Delete it here. Sized-heap pages are deliberately left claimed: that
	 * reservation is permanent and its pages are recycled within Alaska. */
	if (alaska_is_huge_base(backing_base))
	{
		struct big_allocation *b =
			__lookup_bigalloc_from_root(backing_base, &__alaska_heap_allocator, NULL);
		if (b) __liballocs_delete_bigalloc_at(b->begin, &__alaska_heap_allocator);
	}
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
	alaska_resolve_syms();
	if (!pf_alaska_object_base || !pf_alaska_object_size || in_alaska_allocator
			|| !pf_alaska_is_initialized || !pf_alaska_is_initialized())
	{
		return &__liballocs_err_unindexed_heap_object;
	}
	in_alaska_allocator = 1;

	liballocs_err_t err = NULL;
	void *base = pf_alaska_object_base(obj);
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
	unsigned long size = (recorded && rec_size) ? rec_size : pf_alaska_object_size(base);

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
	alaska_resolve_syms();
	if (!pf_alaska_object_base || !pf_alaska_object_size) return 0;
	void *base = pf_alaska_object_base(obj);
	return base ? pf_alaska_object_size(base) : 0;
}


// NOT to be confused with hfree
static void alaska_free(struct allocated_chunk *start)
{
	if(pf_alaska_translate && pf_alaska_hfree_now) {
		if (alaska_is_huge_base(start)) {
			pf_alaska_hfree_now(start);
		} else {
			/**
			 * The Alaska allocator's free on handles is a no-op.
			 * Start here is a raw backing pointer base, not a handle. Hfree needs to be called on a handle to work.
			 * 
			 * We would have to search the handle table, which is too expensive (and not exposed to us). Instead,
			 * we choose to live with some garbage temporarily.
			 * 
			 * Instead of calling hfree directly, we rely on the Alaska garbage collector to call it for us.
			 * Whenever Alaska thinks an object is unreachable, it will call hfree on the handle, ignoring lifetime policy flags.
			 * We then hijack it's hfree call (beloww), and unly actually dispatch to the real hfree if the object has no lp flags
			 */
			
			if(5 <= __liballocs_debug_level) {
				debug_printf(0, "alaska_free called for chunk at %p\n", (void *) start);
			} else {
				void *handle = pf_alaska_handle_for(start);
				debug_printf(0, "DEBUG MODE WARNING: alaska_free called for handle %p -> %p\n", (void *) handle, (void *) start);
				// alaska_hfree_now(handle); // HACK: This really shouldn't be triggered on debug level. 
				
			}
		}
	} else {
			debug_printf(0, "hfree called for %p but we are not ready yet\n", start);
			abort();
	}

}

struct allocator __alaska_heap_allocator = {
	.name = "alaska",
	.min_alignment = 16, /* Alaska aligns allocations to 16 bytes (AlignedSize). */
	/* NOT cacheable: Alaska's GC compacts/relocates objects, so a cached
	 * base/size would go stale after a collection. */
	.is_cacheable = 0,
	.get_info = get_info,
	.get_size = get_size,
	.free = alaska_free,
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
	alaska_resolve_syms();
	if (!pf_alaska_heap_page_extent) return 0;     /* no Alaska runtime loaded */
	/* The Alaska runtime may not be up yet: libalaska's symbols enter the link map
	 * (so fake_dlsym resolves them) the moment it is mapped, but its Runtime/heap are
	 * constructed later. Calling any heap-touching accessor before then crashes -- and
	 * liballocs reaches here while libalaska's own segments are still being mmap'd.
	 * is_initialized() only reads a volatile bool, so it is safe in that window. */
	if (!pf_alaska_is_initialized || !pf_alaska_is_initialized()) return 0;

	/* liballocs calls us for EVERY unindexed mmap -- library segments, the Python
	 * heap, anything. Only addresses inside Alaska's reserved sized heap are ours;
	 * gate on that range up front. This both (a) avoids page_extent's huge-object
	 * path, which is not robust to arbitrary non-Alaska addresses (it crashed on a
	 * library mapping), and (b) covers the still-initialising window: heap_start() is
	 * is_initialized()-gated, returning NULL until the heap is reserved. (Huge Alaska
	 * objects, which live outside the sized heap, are not claimed here -- acceptable:
	 * they are rare and simply fall back to "unknown storage".) */
	{
		void *hs = pf_alaska_heap_start ? pf_alaska_heap_start() : NULL;
		unsigned long hz = pf_alaska_heap_size ? pf_alaska_heap_size() : 0;
		if (!hs || (uintptr_t) ptr < (uintptr_t) hs
				|| (uintptr_t) ptr >= (uintptr_t) hs + hz)
			return 0;
	}
	/* Don't recurse: a nested query raised while we are already talking to
	 * Alaska (e.g. Alaska allocating page-table nodes) -- or while we create the
	 * bigalloc below -- is not ours to claim. Hold the guard across the whole
	 * handler so any such nested entry short-circuits. */
	if (in_alaska_allocator) return 0;
	in_alaska_allocator = 1;

	_Bool ret = 0;
	void *page_base = NULL;
	unsigned long page_size = 0;
	if (pf_alaska_heap_page_extent((void *) ptr, &page_base, &page_size))
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
	if (getenv("RECLAIM_DEBUG"))
	{
		fprintf(stderr, "[pin?] base=%p ins=%p lp=%02x\n",
			base, (void*) ins, (unsigned) ins->common.lifetime_policies);
	}
	return (ins->common.lifetime_policies & MANUAL_DEALLOCATION_FLAG) != 0;
}

/* Hijack hfree 
 *
 *
 * HUGE objects are NOT handles and are NOT GC-managed (Alaska's refcount+stackscan
 * GC only ever sees handles), so detach-instead-of-free would leak them forever --
 * hfree is their only deallocator.
 * 
 */
void hfree(void *ptr)
{
	if (!ptr) return;
	alaska_resolve_syms();
	if (pf_alaska_translate)
	{
		void *base = pf_alaska_translate(ptr);
		if (base != ptr && base)
		{
			// We don't protect ourselves from wild hfrees. Calling pf_alaska_object_base(base) to check might be smart
			__liballocs_detach_lifetime_policy(MANUAL_DEALLOCATION_POLICY, base);
		}
	} else {
		debug_printf(0, "ERROR: hfree called but we can't figure out on whether it's a handle or not. We just caused a memory leak\n");
		abort();
	}
	
}
#endif
