#define _GNU_SOURCE
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <link.h>
#include <dlfcn.h>
#include "allocs.h"
#include "allocmeta.h"

/* Structurally identical types are one object regardless of which meta-DSO holds them */
struct pair { int first; int second; };
struct pair *new_pair(void);
struct pair *canon_pair_addr(void);
int **canon_ptr_addr(void);
int *canon_local_addr(void);
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

/* Every uniqtype pointer that a meta-DSO's uniqtypes hold must lead to the
 * canonical object: the one a global lookup of its name finds. */
static unsigned npointers_checked;
static int check_pointers_canonical(struct uniqtype *t, void *arg)
{
	Dl_info info;
	const ElfW(Sym) *sym = NULL;
	int ret = dladdr1((char *) t + 1, &info, (void **) &sym, RTLD_DL_SYMENT);
	assert(ret != 0 && sym && info.dli_saddr == t);
	unsigned nrel = (sym->st_size - offsetof(struct uniqtype, related))
		/ sizeof (struct uniqtype_rel_info);
	unsigned kind = UNIQTYPE_KIND(t);
	for (unsigned i = 0; i < nrel; ++i)
	{
		if (kind == COMPOSITE && i >= t->un.composite.nmemb) continue; /* names vector */
		if (kind == ENUMERATION && i > 0) continue; /* enumerators */
		struct uniqtype *p = t->related[i].un.t.ptr;
		if (!p) continue;
		Dl_info pinfo;
		ret = dladdr((char *) p + 1, &pinfo);
		assert(ret != 0 && pinfo.dli_sname && pinfo.dli_saddr == p);
		assert(dlsym(RTLD_DEFAULT, pinfo.dli_sname) == p);
		++npointers_checked;
	}
	return 0;
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

	/* Statics: their types come from the metavector in the meta-DSO of the
	 * object that holds them. Ours first... */
	struct uniqtype *static_t = __liballocs_get_alloc_type(&our_pair);
	assert(static_t);
	assert(element_type(static_t) == pair_t);
	struct uniqtype *ptr_t = __liballocs_get_alloc_type(&our_ptr);
	assert(ptr_t);
	assert(UNIQTYPE_IS_POINTER_TYPE(element_type(ptr_t)));
	assert(UNIQTYPE_POINTEE_TYPE(element_type(ptr_t)) == int_t);
	/* ... then libcanon's. Its metavector records name its own copies of
	 * the uniqtypes, we are checking that they were re-written to the
	 * canonical ones. */
	struct uniqtype *lib_static_t = __liballocs_get_alloc_type(canon_pair_addr());
	assert(lib_static_t);
	assert(element_type(lib_static_t) == pair_t);
	struct uniqtype *lib_ptr_t = __liballocs_get_alloc_type(canon_ptr_addr());
	assert(lib_ptr_t);
	assert(UNIQTYPE_IS_POINTER_TYPE(element_type(lib_ptr_t)));
	assert(UNIQTYPE_POINTEE_TYPE(element_type(lib_ptr_t)) == int_t);
	struct uniqtype *lib_local_t = __liballocs_get_alloc_type(canon_local_addr());
	assert(lib_local_t);
	assert(element_type(lib_local_t) == int_t);

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

	/* The canonical definition is the first one loaded: ours, since the
	 * executable's meta-DSO is loaded before libcanon's. */
	struct allocs_file_metadata *exe_meta = __liballocs_get_specific_by_allocator(
		main, &__static_file_allocator, NULL);
	assert(exe_meta && exe_meta->meta_obj_handle);
	assert(dlsym(exe_meta->meta_obj_handle, "__uniqtype__int$$32") == int_t);
	assert(dlsym(exe_meta->meta_obj_handle, "__uniqtype__pair") == pair_t);

	/* ... and so must every pointer in either meta-DSO. One case this catches:
	 * a type libcanon's meta-DSO defines only under a codeless name (a pointer
	 * to a type it sees as incomplete) while ours aliases that name to a
	 * codeful definition. */
	int (*iterate_types)(void *, int (*)(struct uniqtype *, void *), void *)
		= dlsym(RTLD_DEFAULT, "__liballocs_iterate_types");
	assert(iterate_types);
	iterate_types(lib_meta->meta_obj_handle, check_pointers_canonical, NULL);
	iterate_types(exe_meta->meta_obj_handle, check_pointers_canonical, NULL);
	assert(npointers_checked > 100);

	printf("canonical pair uniqtype is %p (%s), libcanon's copy is %p; %u pointers checked\n",
		pair_t, UNIQTYPE_NAME(pair_t), lib_pair_t, npointers_checked);
	return 0;
}
