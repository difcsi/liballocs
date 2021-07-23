#ifndef LIBALLOCS_CIL_INLINES_H_
#define LIBALLOCS_CIL_INLINES_H_

#ifndef unlikely
#define __liballocs_defined_unlikely
#define unlikely(cond) (__builtin_expect( (cond), 0 ))
#endif
#ifndef likely
#define __liballocs_defined_likely
#define likely(cond)   (__builtin_expect( (cond), 1 ))
#endif
#ifndef assert
#define __liballocs_defined_assert
//#ifdef DEBUG
#define assert(cond) \
	if (!(cond)) abort()
//#else
//#define assert(cond)
//#endif
#endif

/* Prototypes we omit. */
void abort(void) __attribute__((noreturn));

/* The functions are *not* weak -- they're defined in the noop library. 
 * we would like the noop library not to be necessary. */
int __liballocs_global_init (void);
/* This is not weak. */
void __assert_fail(const char * assertion, const char * file, unsigned int line, const char * function);
/* Heap index hooks -- these also aren't weak, for the usual reason. */
void __alloca_allocator_notify(void *new_userchunkaddr, unsigned long modified_size, 
		unsigned long *frame_counter, const void *caller, 
		const void *caller_sp, const void *caller_bp);
void __liballocs_index_delete(void*);
struct uniqtype; /* forward decl */

/* This *must* match the size of 'struct insert' in heap_index! But we don't
 * include that header right now, to avoid perturbing the inclusion order
 * of the rest of this translation unit. */
#ifndef ALLOCA_TRAILER_SIZE
#define ALLOCA_TRAILER_SIZE (sizeof (void*))
#endif

/* HACK: copied from memtable.h. */
/* Thanks to Martin Buchholz -- <http://www.wambold.com/Martin/writings/alignof.html> */
#ifndef ALIGNOF
#define ALIGNOF(type) offsetof (struct { char c; type member; }, member)
#endif
#ifndef PAD_TO_ALIGN
#define PAD_TO_ALIGN(n, a) 	((0 == ((n) % (a))) ? (n) : (n) + (a) - ((n) % (a)))
#endif

/* This *must* match the treatment of "early_malloc"'d chunks in malloc_hook_stubs.c. 
 * */
#ifndef ALLOCA_HEADER_SIZE
#define ALLOCA_HEADER_SIZE (sizeof (unsigned long))
#endif

#ifndef NO_TLS
extern __thread void *__current_allocsite __attribute__((weak));
#else
extern void *__current_allocsite __attribute__((weak));
#endif

void __liballocs_unindex_stack_objects_counted_by(unsigned long *, void *frame_addr);

extern inline void (__attribute__((always_inline,gnu_inline,used)) __liballocs_alloca_caller_frame_cleanup)(void *counter);
extern inline void (__attribute__((always_inline,gnu_inline,used)) __liballocs_alloca_caller_frame_cleanup)(void *counter)
{
	__liballocs_unindex_stack_objects_counted_by((unsigned long *) counter, __builtin_frame_address(0));
}

/* alloca helpers */
extern inline const void *(__attribute__((always_inline,gnu_inline,used)) __liballocs_get_sp)(void);
extern inline const void *(__attribute__((always_inline,gnu_inline,used)) __liballocs_get_sp)(void)
{
	unsigned long our_sp;
	#ifdef UNW_TARGET_X86
		__asm__ volatile ("movl %%esp, %0\n" :"=r"(our_sp));
	#else // assume X86_64 for now
		__asm__ volatile ("movq %%rsp, %0\n" : "=r"(our_sp));
	#endif
	return (const void*) our_sp;
}

extern inline const void *(__attribute__((always_inline,gnu_inline,used)) __liballocs_get_bp)(void);
extern inline const void *(__attribute__((always_inline,gnu_inline,used)) __liballocs_get_bp)(void)
{
	return (const void *) __builtin_frame_address(0);
}

extern inline void *(__attribute__((always_inline,gnu_inline,used)) __liballocs_alloca)(unsigned long size, unsigned long *frame_counter, void *caller);
extern inline void *(__attribute__((always_inline,gnu_inline,used)) __liballocs_alloca)(unsigned long size, unsigned long *frame_counter, void *caller)
{
	/* Insert heap trailer etc..
	 * Basically we have to do everything that our malloc hooks, allocator wrappers
	 * and heap indexing code does. ARGH. Maintenance nightmare.... 
	 * 
	 * AND only do the indexing things if liballocs is preloaded. Otherwise.... */
	unsigned long chunk_size = PAD_TO_ALIGN(size + ALLOCA_TRAILER_SIZE, ALLOCA_TRAILER_SIZE);
	void *alloc = __builtin_alloca(ALLOCA_HEADER_SIZE + chunk_size);
#ifndef LIBALLOCS_NO_ZERO
	__builtin_memset((char*) alloc + ALLOCA_HEADER_SIZE, 0, chunk_size);
#endif
	/* write the usable size into the first word, then return the rest. */
	*(unsigned long *)alloc = chunk_size;
	
	/* We add only the "usable size" part, because that is what the heap index code
	 * can see, and that is the code that will be consuming this value. */
	*frame_counter += chunk_size;
	
	/* Note that we pass the caller directly; __current_allocsite is not required. */
	void *userptr = (char*) alloc + ALLOCA_HEADER_SIZE;
	__alloca_allocator_notify(userptr, chunk_size, frame_counter, caller,
		__liballocs_get_sp(), __liballocs_get_bp());
	
	return userptr;
}

void __liballocs_unindex_stack_objects_below(void *);

extern _Bool __liballocs_is_initialized __attribute__((weak));

/* Our cache design must integrate bounds and types.
 * It is a bit non-obvious.
 * The idea is to remember the regularity of memory, so as to
 * "cache many type facts at once" in the case of arrays.
 * But it's not always the array element type that we care about;
 * it might be a subobject within the element
 * (say, if the program is repeatedly hitting those subobjects).
 * A "memrange" is a generalisation of arrays.
 * For a memrange, we record its start and end, and also its "period"
 * which is the element type size.
 * Then, separately, we record the offset from one period-sized
 * subdivision *to* a t.
 * In this way, we can cache the same array of structs (say) many times,
 * each time recording a different element type,
 * maybe buried deep in a nest of structs.
 *
 * Our cache entries are useful for checking bounds, in the case where
 * period == size of t.
 */
struct __liballocs_memrange_cache_entry_s
{
	const void *range_base;
	const void *range_limit;
	unsigned period; // range_base to range_limit must be a multiple of "period" bytes
	unsigned offset_to_t; // at this offset within any period, we have a t
	/*struct uniqtype * */ unsigned long t:48;
	unsigned char prev_mru;
	unsigned char next_mru;
	/* TODO: do inline uniqtype cache word check? */
} __attribute__((aligned(64)));

#ifndef LIBALLOCS_MEMRANGE_CACHE_MAX_SIZE
#define LIBALLOCS_MEMRANGE_CACHE_MAX_SIZE 8
#endif
struct __liballocs_memrange_cache
{
	/* We use index 0 to mean "unused" / "null". */
	struct __liballocs_memrange_cache_entry_s entries[1 + LIBALLOCS_MEMRANGE_CACHE_MAX_SIZE];
	const unsigned char max_pos; /* was "size_plus_one", i.e. size including the null entry */
	unsigned char next_victim;
	unsigned char head_mru;
	unsigned char tail_mru;
	unsigned short validity; /* does *not* include the null entry */
} __attribute__((aligned(64)));
extern struct __liballocs_memrange_cache /* __thread */ __liballocs_ool_cache;

extern inline void (__attribute__((always_inline,gnu_inline,used)) __liballocs_check_cache_sanity )(struct __liballocs_memrange_cache *cache __attribute__((unused)));
extern inline void (__attribute__((always_inline,gnu_inline,used)) __liballocs_check_cache_sanity )(struct __liballocs_memrange_cache *cache __attribute__((unused)))
{
#ifdef DEBUG
	unsigned visited_linear = 0u;
	for (int i = 1; i <= cache->max_pos; ++i)
	{
		if (cache->validity & (1<<(i-1)))
		{
			visited_linear |= (1<<(i-1));
		}
	}
#ifndef LIBALLOCS_CACHE_LINEAR
	unsigned visited_mru = 0u;
	for (unsigned char i = cache->head_mru; i != 0; i = cache->entries[i].next_mru)
	{
		assert(cache->validity & (1<<(i-1)));
		// assert we haven't been here before
		assert(!(visited_mru & (1<<(i-1))));
		visited_mru |= (1<<(i-1));
	}
	assert(visited_linear == visited_mru);
	// go the other way too
	unsigned visited_lru = 0u;
	for (unsigned char i = cache->tail_mru; i != 0; i = cache->entries[i].prev_mru)
	{
		assert(cache->validity & (1<<(i-1)));
		// assert we haven't been here before
		assert(!(visited_lru & (1<<(i-1))));
		visited_lru |= (1<<(i-1));
	}
	assert(visited_linear == visited_lru);
#endif
	/* No two entries should have the same start, end, period, offset and t. */
	for (int i = 1; i <= cache->max_pos; ++i)
	{
		for (int j = i+1; j <= cache->max_pos; ++j)
		{
			if ((cache->validity & (1<<(i-1)))
					&& (cache->validity & (1<<(j-1))))
			{
				assert(!((cache->entries[i].range_base == cache->entries[j].range_base)
				&& (cache->entries[i].range_limit == cache->entries[j].range_limit)
				&& (cache->entries[i].period == cache->entries[j].period)
				&& (cache->entries[i].offset_to_t == cache->entries[j].offset_to_t)
				&& (cache->entries[i].t == cache->entries[j].t)));
			}
		}
	}
#endif
}

#ifndef LIBALLOCS_CACHE_LINEAR
extern inline void (__attribute__((always_inline,gnu_inline,used)) __liballocs_cache_unlink_mru )(struct __liballocs_memrange_cache *cache, unsigned i);
extern inline void (__attribute__((always_inline,gnu_inline,used)) __liballocs_cache_unlink_mru )(struct __liballocs_memrange_cache *cache, unsigned i)
{
	// unhook us from the mru list
	unsigned char our_next = cache->entries[i].next_mru;
	unsigned char our_prev = cache->entries[i].prev_mru;
	if (our_prev) cache->entries[our_prev].next_mru = our_next;
	if (our_next) cache->entries[our_next].prev_mru = our_prev;
	if (cache->head_mru == i) cache->head_mru = our_next;
	if (cache->tail_mru == i) cache->tail_mru = our_prev;
}

extern inline void (__attribute__((always_inline,gnu_inline,used)) __liballocs_cache_link_head_mru )(struct __liballocs_memrange_cache *cache, unsigned i);
extern inline void (__attribute__((always_inline,gnu_inline,used)) __liballocs_cache_link_head_mru )(struct __liballocs_memrange_cache *cache, unsigned i)
{
	/* Put us at the head of the LRU chain. */
	cache->entries[i].prev_mru = 0;
	cache->entries[i].next_mru = cache->head_mru;
	/* Link us in at the head. */
	if (cache->head_mru != 0) cache->entries[cache->head_mru].prev_mru = (unsigned char) i;
	cache->head_mru = (unsigned char) i;
	/* Set the tail, if we didn't already have one. */
	if (cache->tail_mru == 0) cache->tail_mru = i;
	/* Sanity: assert we're not trivially circular. */
	assert(cache->entries[i].next_mru != i);
	assert(cache->entries[i].prev_mru != i);
}
#endif

extern inline void (__attribute__((always_inline,gnu_inline,used)) __liballocs_cache_invalidate )(struct __liballocs_memrange_cache *cache, unsigned i);
extern inline void (__attribute__((always_inline,gnu_inline,used)) __liballocs_cache_invalidate )(struct __liballocs_memrange_cache *cache, unsigned i)
{
	__liballocs_check_cache_sanity(cache);
	// unset validity and make this the next victim
	cache->validity &= ~(1u<<(i-1));
	cache->next_victim = i;
	// unhook us from the mru list
#ifndef LIBALLOCS_CACHE_LINEAR
	__liballocs_cache_unlink_mru(cache, i);
#else
	cache->next_victim = i;
#endif
	/* We're definitely invalid. */
	cache->validity &= ~(1u<<(i-1));
	__liballocs_check_cache_sanity(cache);
}

extern inline void (__attribute__((always_inline,gnu_inline,used)) __liballocs_cache_bump )(struct __liballocs_memrange_cache *cache, unsigned i);
extern inline void (__attribute__((always_inline,gnu_inline,used)) __liballocs_cache_bump )(struct __liballocs_memrange_cache *cache, unsigned i)
{
	__liballocs_check_cache_sanity(cache);
#ifdef LIBALLOCS_CACHE_LINEAR
	// make sure we're not the next victim
	if (unlikely(cache->next_victim == i))
	{
		if (cache->max_pos > 1)
		{
			cache->next_victim = 1 + ((i + 1 - 1) % (cache->max_pos - 1));
		}
	}
#else /* MRU */
	if (cache->head_mru != i)
	{
		__liballocs_cache_unlink_mru(cache, i);
		__liballocs_cache_link_head_mru(cache, i);
	}
#endif
	__liballocs_check_cache_sanity(cache);
}

/* HMM. This will cause problems if someone includes uniqtype-defs.h
 * *after* they include liballocs_cil_inlines.h. */
#ifndef UNIQTYPE_DECLS
struct alloc_addr_info
{
	unsigned long addr:47;
	unsigned flag:1;
	unsigned bits:16;
};
#endif

#define UNIQTYPE_QD_KIND(t) \
(*((char*)(unsigned long)(t) + 0xc) & 0xf)
#define UNIQTYPE_QD_CACHE_WORD(t) \
(*(struct alloc_addr_info *)(unsigned long)(t))
#define UNIQTYPE_QD_RELATED0_T(t) \
({ struct uniqtype *related; \
	__builtin_memcpy((char*) &related, (char*)(unsigned long)(t) + 0x18, 8); \
	related; })
#define UNIQTYPE_QD_SIZE(t) \
({ unsigned pos_maxoff; \
	__builtin_memcpy((char*) &pos_maxoff, (char*)(unsigned long)(t) + 0x8, 4); \
	pos_maxoff; })
#define UNIQTYPE_QD_IS_ARRAY(t) \
	(((UNIQTYPE_QD_KIND(t)) & 0x1) == 1)
#define UNIQTYPE_QD_ARRAY_ELEMENT_SIZE(t) \
	(UNIQTYPE_QD_SIZE(UNIQTYPE_QD_RELATED0_T(t)))

extern inline
struct __liballocs_memrange_cache_entry_s *(__attribute__((always_inline,gnu_inline,used))
__liballocs_memrange_cache_lookup_with_type )(struct __liballocs_memrange_cache *cache, const void *obj, struct uniqtype *query_t, unsigned long query_period);
extern inline
struct __liballocs_memrange_cache_entry_s *(__attribute__((always_inline,gnu_inline,used))
__liballocs_memrange_cache_lookup_with_type )(struct __liballocs_memrange_cache *cache, const void *obj, struct uniqtype *query_t, unsigned long query_period)
{
#ifndef LIBALLOCS_NOOP_INLINES
	__liballocs_check_cache_sanity(cache);
#ifdef LIBALLOCS_CACHE_LINEAR
	for (unsigned char i = 1; i <= cache->max_pos; ++i)
#else
	for (unsigned char i = cache->head_mru; i != 0; i = cache->entries[i].next_mru)
#endif
	{
		if (cache->validity & (1<<(i-1)))
		{
			/* We test whether the object is in-bounds, at an offset
			 * that is congruent to the offset-to-a-t (modulo the entry period)
			 * and, if a query period is passed, that the period matches exactly
			 * (or ditto for the type)... this is for bounds checks, since we
			 * can only do indexing in an array if the element size matches
			 * the pointer's target size.
			 *
			 * Since division is probably the slowest part of the test, we
			 * save it until last. */

/* There are many memranges we can match against.
 * (1) the top-level range of the cache entry
 * (2a) the "offset range" described by the offset-to-t and t fields,
 *          which may or may not be the same as the top-level range
 *       (it is the same iff offset == 0 and period == t->pos_maxoff).
 * (2b) the "offset range" described by offset-to-t and t fields,
 *          if t is an array type
 *       (sanity suggests we should only do this if offset is nonzero)
 *       (FIXME: include this in the sanity checks)
 *       (for types-only libcrunch we never use array types in cache entries,
 *        but this will change for bounds checking)
 * (3a) the "cached contained range" described by the uniqtype cache word,
 *        in the case where the cached type is a singleton
 * (3b) the "cached contained range"... where it's an array. */
#define UNPACK_TYPE(__t) ((struct uniqtype *) (unsigned long) (__t))
#define ENTRY_TYPE (UNPACK_TYPE(cache->entries[i].t))
#define MODULUS (diff % cache->entries[i].period)
#define ENTRY_TYPE_MATCHES_TOPLEVEL_RANGE ( \
	(ENTRY_TYPE == query_t) && \
	(MODULUS == 0))
#define ENTRY_TYPE_MATCHES_OFFSET_RANGE_SINGLETON ( \
	(ENTRY_TYPE == query_t) && \
	(MODULUS == cache->entries[i].offset_to_t))
#define ENTRY_TYPE_MATCHES_OFFSET_RANGE_ARRAY ( \
		( (ENTRY_TYPE) && \
			UNIQTYPE_QD_IS_ARRAY( ENTRY_TYPE ) && \
			((MODULUS - cache->entries[i].offset_to_t) % \
				UNIQTYPE_QD_ARRAY_ELEMENT_SIZE( ENTRY_TYPE ) == 0 \
				/* (modulus - offset) is a multiple of element size */) \
		) \
	)
#define CACHED_CONTAINED_RANGE_TYPE_MATCHES_SINGLETON ( \
	(ENTRY_TYPE) && \
	( \
		( \
			(query_t == UNPACK_TYPE(UNIQTYPE_QD_CACHE_WORD(ENTRY_TYPE).addr) ) && \
			(MODULUS - cache->entries[i].offset_to_t == UNIQTYPE_QD_CACHE_WORD(ENTRY_TYPE).bits /* modulus matches offset */) \
		) \
	))
#define CACHED_CONTAINED_RANGE_TYPE_MATCHES_ARRAY ( \
	(ENTRY_TYPE) && \
		( UNPACK_TYPE(UNIQTYPE_QD_CACHE_WORD(ENTRY_TYPE).addr) && \
			UNIQTYPE_QD_IS_ARRAY( UNPACK_TYPE(UNIQTYPE_QD_CACHE_WORD(ENTRY_TYPE).addr) ) && \
			((MODULUS - cache->entries[i].offset_to_t - UNIQTYPE_QD_CACHE_WORD(ENTRY_TYPE).bits) % \
				UNIQTYPE_QD_ARRAY_ELEMENT_SIZE(UNPACK_TYPE(UNIQTYPE_QD_CACHE_WORD(ENTRY_TYPE).addr)) == 0 \
				/* (modulus - offset) is a multiple of element size */) \
		) \
	)
			signed long long diff = (char*) obj - (char*) cache->entries[i].range_base;
			if ((char*) obj >= (char*)cache->entries[i].range_base
					&& (char*) obj < (char*)cache->entries[i].range_limit
					&& (!query_period || cache->entries[i].period == query_period)
					&& (!query_t || ENTRY_TYPE_MATCHES_TOPLEVEL_RANGE
						|| ENTRY_TYPE_MATCHES_OFFSET_RANGE_SINGLETON
						|| ENTRY_TYPE_MATCHES_OFFSET_RANGE_ARRAY
						|| CACHED_CONTAINED_RANGE_TYPE_MATCHES_SINGLETON
						|| CACHED_CONTAINED_RANGE_TYPE_MATCHES_ARRAY))
			{
				// hit
				__liballocs_cache_bump(cache, i);
				__liballocs_check_cache_sanity(cache);
				return &cache->entries[i];
			}
		}
	}
#endif
	__liballocs_check_cache_sanity(cache);
	return (struct __liballocs_memrange_cache_entry_s *)(void*)0;
}

extern inline
struct __liballocs_memrange_cache_entry_s *(__attribute__((always_inline,gnu_inline,used))
__liballocs_memrange_cache_lookup_with_size )(struct __liballocs_memrange_cache *cache, const void *obj, unsigned query_p);

/* The query period (target type size) must match the range period.
 * If the range is a singleton, its period is the whole range's type's size.
 * Note that we ignore t and offset_to_t in the simpler case.
 * We don't necessarily know the type of the whole range.
 * In the type-match case, we require offset to be 0 and t to be equal. */
#define QUERY_START_MATCHES_TOPLEVEL_RANGE(__e, __p) ( \
		(MODULUS == 0))
#define QUERY_SIZE_MATCHES_TOPLEVEL_RANGE(__e, __p, __qp) ( \
		QUERY_START_MATCHES_TOPLEVEL_RANGE((__e), (__p)) && \
		((__qp) == (__e).period))

#define QUERY_START_MATCHES_OFFSET_RANGE_SINGLETON(__e, __p) ( \
		(MODULUS == (__e).offset_to_t))
#define QUERY_SIZE_MATCHES_OFFSET_RANGE_SINGLETON(__e, __p, __qp) ( \
		QUERY_START_MATCHES_OFFSET_RANGE_SINGLETON((__e), (__p)) && \
		(UNIQTYPE_QD_SIZE(ENTRY_TYPE) == (__qp)))

#define QUERY_START_MATCHES_OFFSET_RANGE_ARRAY(__e, __p) ( \
				(ENTRY_TYPE) && \
						UNIQTYPE_QD_IS_ARRAY( ENTRY_TYPE ) && \
						((MODULUS - (__e).offset_to_t) % \
								UNIQTYPE_QD_ARRAY_ELEMENT_SIZE( ENTRY_TYPE ) == 0 \
								/* (modulus - offset) is a multiple of element size */) \
		)
#define QUERY_SIZE_MATCHES_OFFSET_RANGE_ARRAY(__e, __p, __qp) ( \
		QUERY_START_MATCHES_OFFSET_RANGE_ARRAY((__e), (__p)) && \
				((__qp) == UNIQTYPE_QD_ARRAY_ELEMENT_SIZE( ENTRY_TYPE )))

/* Now it's the uniqtype-cached contained range that must match. */
#define QUERY_START_MATCHES_CACHED_CONTAINED_RANGE_SINGLETON(__e, __p) ( \
		ENTRY_TYPE && \
		( \
						(MODULUS - (__e).offset_to_t == UNIQTYPE_QD_CACHE_WORD(ENTRY_TYPE).bits /* modulus matches offset */) \
		))
#define QUERY_SIZE_MATCHES_CACHED_CONTAINED_RANGE_SINGLETON(__e, __p, __qp) ( \
		QUERY_START_MATCHES_CACHED_CONTAINED_RANGE_SINGLETON((__e), (__p)) && \
		(ENTRY_TYPE) && \
				(UNPACK_TYPE(UNIQTYPE_QD_CACHE_WORD(ENTRY_TYPE).addr)) && \
				((__qp) == UNIQTYPE_QD_SIZE(UNPACK_TYPE(UNIQTYPE_QD_CACHE_WORD(ENTRY_TYPE).addr) )) \
		)

#define QUERY_START_MATCHES_CACHED_CONTAINED_RANGE_ARRAY(__e, __p) ( \
		(ENTRY_TYPE) && \
				( UNPACK_TYPE(UNIQTYPE_QD_CACHE_WORD(ENTRY_TYPE).addr) && \
						UNIQTYPE_QD_IS_ARRAY( UNPACK_TYPE(UNIQTYPE_QD_CACHE_WORD(ENTRY_TYPE).addr) ) && \
						((MODULUS - (__e).offset_to_t - UNIQTYPE_QD_CACHE_WORD(ENTRY_TYPE).bits) % \
								UNIQTYPE_QD_ARRAY_ELEMENT_SIZE(UNPACK_TYPE(UNIQTYPE_QD_CACHE_WORD(ENTRY_TYPE).addr)) == 0 \
								/* (modulus - offset) is a multiple of element size */) \
				) \
		)
#define QUERY_SIZE_MATCHES_CACHED_CONTAINED_RANGE_ARRAY(__e, __p, __qp) ( \
		QUERY_START_MATCHES_CACHED_CONTAINED_RANGE_ARRAY((__e), (__p)) && \
		UNIQTYPE_QD_ARRAY_ELEMENT_SIZE(UNPACK_TYPE(UNIQTYPE_QD_CACHE_WORD(ENTRY_TYPE).addr)) == (__qp) \
)


extern inline
struct __liballocs_memrange_cache_entry_s *(__attribute__((always_inline,gnu_inline,used))
__liballocs_memrange_cache_lookup_with_size )(struct __liballocs_memrange_cache *cache, const void *obj, unsigned query_p)
{
	assert(query_p);
	if (!query_p) __builtin_unreachable();
#ifndef LIBALLOCS_NOOP_INLINES
	__liballocs_check_cache_sanity(cache);
#ifdef LIBALLOCS_CACHE_LINEAR
	for (unsigned char i = 1; i <= cache->max_pos; ++i)
#else
	for (unsigned char i = cache->head_mru; i != 0; i = cache->entries[i].next_mru)
#endif
	{
		if (cache->validity & (1<<(i-1)))
		{
			signed long long diff = (char*) obj - (char*) cache->entries[i].range_base;
			if ((char*) obj >= (char*)cache->entries[i].range_base
					&& (char*) obj < (char*)cache->entries[i].range_limit
					&& (QUERY_SIZE_MATCHES_TOPLEVEL_RANGE(cache->entries[i], obj, query_p)
						|| QUERY_SIZE_MATCHES_OFFSET_RANGE_SINGLETON(cache->entries[i], obj, query_p)
						|| QUERY_SIZE_MATCHES_OFFSET_RANGE_ARRAY(cache->entries[i], obj, query_p)
						|| QUERY_SIZE_MATCHES_CACHED_CONTAINED_RANGE_SINGLETON(cache->entries[i], obj, query_p)
						|| QUERY_SIZE_MATCHES_CACHED_CONTAINED_RANGE_ARRAY(cache->entries[i], obj, query_p)))

			{
				// hit
				__liballocs_cache_bump(cache, i);
				__liballocs_check_cache_sanity(cache);
				return &cache->entries[i];
			}
		}
	}
#endif
	__liballocs_check_cache_sanity(cache);
	return (struct __liballocs_memrange_cache_entry_s *)(void*)0;
}


extern inline
struct __liballocs_memrange_cache_entry_s *(__attribute__((always_inline,gnu_inline,used))
__liballocs_memrange_cache_lookup_range )(struct __liballocs_memrange_cache *cache, const void *obj);
extern inline
struct __liballocs_memrange_cache_entry_s *(__attribute__((always_inline,gnu_inline,used))
__liballocs_memrange_cache_lookup_range )(struct __liballocs_memrange_cache *cache, const void *obj)
{
#ifndef LIBALLOCS_NOOP_INLINES
	__liballocs_check_cache_sanity(cache);
#ifdef LIBALLOCS_CACHE_LINEAR
	for (unsigned char i = 1; i <= cache->max_pos; ++i)
#else
	for (unsigned char i = cache->head_mru; i != 0; i = cache->entries[i].next_mru)
#endif
	{
		if (cache->validity & (1<<(i-1)))
		{
			if ((char*) obj >= (char*)cache->entries[i].range_base
					&& (char*) obj < (char*)cache->entries[i].range_limit)
			{
				// hit
				__liballocs_cache_bump(cache, i);
				__liballocs_check_cache_sanity(cache);
				return &cache->entries[i];
			}
		}
	}
#endif
	__liballocs_check_cache_sanity(cache);
	return (struct __liballocs_memrange_cache_entry_s *)(void*)0;
}

extern inline struct uniqtype *(__attribute__((always_inline,gnu_inline,used)) __liballocs_get_cached_object_type)(const void *addr);
extern inline struct uniqtype *(__attribute__((always_inline,gnu_inline,used)) __liballocs_get_cached_object_type)(const void *addr)
{
	struct __liballocs_memrange_cache_entry_s *found = __liballocs_memrange_cache_lookup_with_type(
		&__liballocs_ool_cache,
		addr,
		(void*)0,
		0);
	/* This will give us "zero-offset matches", but not contained matches. 
	 * I.e. we know that "addr" is a "found->uniqtype", but we pass over
	 * cases where some cached allocation spans "addr" at a non-zero offset. */
	if (found) return (struct uniqtype *) (unsigned long) found->t;
	return (struct uniqtype *)(void*)0;
}

void __liballocs_uncache_all(const void *allocptr, unsigned long size);

extern inline void
(__attribute__((always_inline,gnu_inline))  __liballocs_trace_cache_eviction)
	(struct __liballocs_memrange_cache_entry_s *old,
	struct __liballocs_memrange_cache_entry_s *new)
{
#ifdef LIBALLOCS_TRACE_CACHE_EVICTION
	__liballocs_trace_cache_eviction_ool(old, new);
#endif
}

extern inline void
(__attribute__((always_inline,gnu_inline)) __liballocs_cache_with_type)(
	struct __liballocs_memrange_cache *c,
	const void *range_base, const void *range_limit, unsigned period,
	unsigned offset_to_t, const struct uniqtype *t, unsigned short depth)
{
	__liballocs_check_cache_sanity(c);
	assert(!period || ((unsigned long) range_limit - (unsigned long) range_base) % period == 0);
	_Bool evicting = 0;
#ifdef LIBALLOCS_CACHE_REPLACE_FIFO
	unsigned pos = c->next_victim;
	evicting = (c->validity & (1u<<(pos-1)));
	c->validity |= (1u<<(pos-1));
#else /* replace MRUwise */
	// First try to find an unused slot.
	// __builtin_ffs returns "one plus the index of the least significant 1-bit"
	// ... which is almost exactly what we want. But we want 0-bit, not 1-bit.
	unsigned pos = __builtin_ffs(~(c->validity));
	assert(pos <= c->max_pos);
	if (pos != c->max_pos)
	{
		// we found a free slot
		assert(!(c->validity & (1u<<(pos-1))));
		// chain it into the MRU list at the head, so that
		// we can assume from now on that we're somewhere in the list
		c->validity |= (1u<<(pos-1));
		__liballocs_cache_link_head_mru(c, pos);
		//c->entries[pos].prev_mru = 0;
		//c->entries[pos].next_mru = c->head_mru;
		//c->head_mru = pos;
		//if (!c->tail_mru) c->tail_mru = pos;
	}
	else
	{
		// we pick the tail slot, but preemptively bump it to the top
		// so that our "same position" is always the right one
		pos = c->tail_mru;
		evicting = 1;
		assert(pos != 0);
		__liballocs_cache_bump(c, pos);
	}
#endif
	struct __liballocs_memrange_cache_entry_s new_c = (struct __liballocs_memrange_cache_entry_s) {
		.range_base = range_base,
		.range_limit = range_limit,
		.period = period,
		.offset_to_t = offset_to_t,
		.t = (unsigned long) (struct uniqtype *) t,
		.prev_mru = c->entries[pos].prev_mru, // insert in the same position in the MRU chain...
		.next_mru = c->entries[pos].next_mru//,
		//.depth = depth
	};
	// install it
	if (evicting) __liballocs_trace_cache_eviction(&c->entries[pos], &new_c);
	c->entries[pos] = new_c;
#ifdef LIBALLOCS_CACHE_REPLACE_FIFO
	// bump us to the top -- if we're MRU, we did this earlier
	__liballocs_cache_bump(c, pos);
#endif
	__liballocs_check_cache_sanity(c);
}

#ifdef __liballocs_defined_unlikely
#undef unlikely
#endif
#ifdef __liballocs_defined_likely
#undef likely
#endif
#ifdef __liballocs_defined_assert
#undef assert
#endif

#endif
