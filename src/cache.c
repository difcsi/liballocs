#define _GNU_SOURCE
#include "liballocs.h"
#include "liballocs_private.h"
#include "pageindex.h"
#include "liballocs_cil_inlines.h"

// FIXME: this should be thread-local but my gdb can't grok that
struct __liballocs_memrange_cache /* __thread */ __liballocs_ool_cache = {
	.max_pos = 1 + LIBALLOCS_MEMRANGE_CACHE_MAX_SIZE,
	.next_victim = 1
};

void __liballocs_uncache_all(const void *allocptr, unsigned long size)
{
	__liballocs_check_cache_sanity(&__liballocs_ool_cache);
	for (unsigned i = 1; i < __liballocs_ool_cache.max_pos; ++i)
	{
		if (__liballocs_ool_cache.validity & (1u << (i-1)))
		{
			assert((__liballocs_check_cache_sanity(&__liballocs_ool_cache), 1));
			/* Uncache any object beginning anywhere within the passed-in range. */
			if ((char*) __liballocs_ool_cache.entries[i].range_base >= (char*) allocptr
					 && (char*) __liballocs_ool_cache.entries[i].range_base < (char*) allocptr + size)
			{
				// unset validity and make this the next victim
				__liballocs_cache_invalidate(&__liballocs_ool_cache, i);
			}
			__liballocs_check_cache_sanity(&__liballocs_ool_cache);
		}
	}
	__liballocs_check_cache_sanity(&__liballocs_ool_cache);
}

void __liballocs_trace_cache_eviction_ool(struct __liballocs_memrange_cache_entry_s *old,
	struct __liballocs_memrange_cache_entry_s *new)
{
	debug_printf(0, "Evicting memrange %p-%p (size %ld; has type %s"" at offset %d"") "
		"in favour of %p-%p (size %ld; has type %s"" at offset %d"")\n",
		old->range_base, old->range_limit, (long) ((char*) old->range_limit - (char*) old->range_base),
			NAME_FOR_UNIQTYPE((struct uniqtype *)(unsigned long) old->t), (int) old->offset_to_t,
		new->range_base, new->range_limit, (long) ((char*) new->range_limit - (char*) new->range_base),
			NAME_FOR_UNIQTYPE((struct uniqtype *)(unsigned long) new->t), (int) new->offset_to_t
	);
}

void __noop(void *arg) {}

//_Bool query_start_matches_toplevel_range(struct __liballocs_memrange_cache_entry_s *e, const void *p)
//{ return QUERY_START_MATCHES_TOPLEVEL_RANGE(*e, p); }

//_Bool query_size_matches_toplevel_range(struct __liballocs_memrange_cache_entry_s *e, const void *p, unsigned qp)
//{ return QUERY_SIZE_MATCHES_TOPLEVEL_RANGE(*e, p, qp); }

//_Bool query_type_matches_toplevel_range(struct __liballocs_memrange_cache_entry_s *e, const void *p, struct uniqtype *qt)
//{ return QUERY_TYPE_MATCHES_TOPLEVEL_RANGE(*e, p, qt); }

//_Bool query_start_matches_offset_range_singleton(struct __liballocs_memrange_cache_entry_s *e, const void *p)
//{ return QUERY_START_MATCHES_OFFSET_RANGE_SINGLETON(*e, p); }

//_Bool query_size_matches_offset_range_singleton(struct __liballocs_memrange_cache_entry_s *e, const void *p, unsigned qp)
//{ return QUERY_SIZE_MATCHES_OFFSET_RANGE_SINGLETON(*e, p, qp); }

//_Bool query_type_matches_offset_range_singleton(struct __liballocs_memrange_cache_entry_s *e, const void *p, struct uniqtype *qt)
//{ return QUERY_TYPE_MATCHES_OFFSET_RANGE_SINGLETON(*e, p, qt); }

