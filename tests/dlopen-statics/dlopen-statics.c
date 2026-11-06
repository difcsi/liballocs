#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "allocs.h"

#ifndef LIBDLSTAT_PATH
#error "LIBDLSTAT_PATH must name libdlstat.so (see mk.inc)"
#endif

struct pair;

static struct uniqtype *element_type(struct uniqtype *t)
{
	return UNIQTYPE_IS_ARRAY_TYPE(t) ? UNIQTYPE_ARRAY_ELEMENT_TYPE(t) : t;
}

int main(void)
{
	/* The static-symbol allocator answers from the per-segment metavectors
	 * that come from the object's meta-DSO. For a dlopen'd object, that
	 * meta-DSO is loaded only after the segments have been defined. */
	void *h = dlopen(LIBDLSTAT_PATH, RTLD_NOW);
	assert(h);
	struct pair *(*pair_addr)(void) = dlsym(h, "dlstat_pair_addr");
	int *(*local_addr)(void) = dlsym(h, "dlstat_local_addr");
	assert(pair_addr && local_addr);

	struct uniqtype *pair_t = __liballocs_get_alloc_type(pair_addr());
	assert(pair_t);
	assert(UNIQTYPE_IS_COMPOSITE_TYPE(element_type(pair_t)));
	assert(0 == strcmp(UNIQTYPE_NAME(element_type(pair_t)), "pair"));

	struct uniqtype *int_t = dlsym(RTLD_DEFAULT, "__uniqtype__int$$32");
	assert(int_t);
	struct uniqtype *local_t = __liballocs_get_alloc_type(local_addr());
	assert(local_t);
	assert(element_type(local_t) == int_t);

	printf("dlopen'd statics: pair is %s, local int is %s\n",
		UNIQTYPE_NAME(element_type(pair_t)), UNIQTYPE_NAME(element_type(local_t)));

	/* Unloading the library must not unmap its meta-DSO: a uniqtype is one
	 * object for the whole process, so other objects' metadata (and we)
	 * may hold pointers into it. */
	int ret = dlclose(h);
	assert(ret == 0);
	struct uniqtype *pair_elem_t = element_type(pair_t);
	assert(0 == strcmp(UNIQTYPE_NAME(pair_elem_t), "pair"));
	assert(pair_elem_t->un.composite.nmemb == 2);
	assert(pair_elem_t->related[0].un.memb.ptr == int_t);
	return 0;
}
