#define _GNU_SOURCE
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <link.h>
#include <dlfcn.h>
#include "allocs.h"
#include "allocmeta.h"

/* Structurally identical types are one object regardless of which meta-DSO holds them */
struct pair { int first; int second; };
struct pair *new_pair(void);
extern struct pair canon_static_pair;
extern int *canon_static_ptr;
struct pair our_pair = { 3, 4 };
int *our_ptr = &our_pair.first;

static struct uniqtype *canonical(const char *name)
{
	struct uniqtype *u = dlsym(RTLD_DEFAULT, name);
	assert(u);
	return u;
}
static struct uniqtype *element_type(struct uniqtype *t)
{
	return UNIQTYPE_IS_ARRAY_TYPE(t) ? UNIQTYPE_ARRAY_ELEMENT_TYPE(t) : t;
}

int main(void)
{
	struct uniqtype *pair_t = canonical("__uniqtype__pair");
	struct uniqtype *int_t = canonical("__uniqtype__int$$32");

	/* A heap allocation: its type comes from our meta-DSO's allocsites table. */
	struct pair *p = malloc(sizeof (struct pair));
	assert(p);
	struct uniqtype *heap_t = __liballocs_get_alloc_type(p);
	assert(heap_t);
	assert(element_type(heap_t) == pair_t);

	/* Statics: their types come from our meta-DSO's metavector. (Those of
	 * the lib, a DT_NEEDED library, get no metavector: its
	 * segments are set up before its meta-DSO is loaded.) */
	struct uniqtype *static_t = __liballocs_get_alloc_type(&our_pair);
	assert(static_t);
	assert(element_type(static_t) == pair_t);
	struct uniqtype *ptr_t = __liballocs_get_alloc_type(&our_ptr);
	assert(ptr_t);
	assert(UNIQTYPE_IS_POINTER_TYPE(element_type(ptr_t)));
	assert(UNIQTYPE_POINTEE_TYPE(element_type(ptr_t)) == int_t);

	/* Uniqtype-to-uniqtype pointers inside THE LIB's own meta-DSO: look up
	 * its copy of pair there, and check its member type is the canonical int. */
	struct allocs_file_metadata *lib_meta = __liballocs_get_specific_by_allocator(
		new_pair, &__static_file_allocator, NULL);
	assert(lib_meta && lib_meta->meta_obj_handle);
	struct uniqtype *lib_pair_t = dlsym(lib_meta->meta_obj_handle, "__uniqtype__pair");
	assert(lib_pair_t);
	assert(UNIQTYPE_IS_COMPOSITE_TYPE(lib_pair_t));
	assert(lib_pair_t->related[0].un.memb.ptr == int_t);
	assert(lib_pair_t->related[1].un.memb.ptr == int_t);

	printf("canonical pair uniqtype is %p (%s), libcanon's copy is %p\n",
		pair_t, UNIQTYPE_NAME(pair_t), lib_pair_t);
	return 0;
}
