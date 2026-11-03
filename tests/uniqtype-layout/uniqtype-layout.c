#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <link.h>
#include <dlfcn.h>
#include "allocs.h"

/* Pins the uniqtype layout that meta-DSOs are generated with: an 8-byte
 * header (pos_maxoff and the kind-specific word) followed by 16-byte
 * related[] entries. There is no per-uniqtype cache word or make_precise
 * pointer: whether a type can make itself precise follows from its kind
 * (today: unbounded arrays, using the object's memory bounds). */

int an_int = 42;

static size_t uniqtype_symbol_size(struct uniqtype *u)
{
	Dl_info info;
	const ElfW(Sym) *sym = NULL;
	int ret = dladdr1((char *) u + 1, &info, (void **) &sym, RTLD_DL_SYMENT);
	assert(ret != 0 && sym && info.dli_saddr == u);
	return sym->st_size;
}

int main(void)
{
	assert(offsetof(struct uniqtype, related) == 8);
	assert(sizeof (struct uniqtype_rel_info) == 16);

	struct uniqtype *int_t = dlsym(RTLD_DEFAULT, "__uniqtype__int$$32");
	assert(int_t);
	assert(uniqtype_symbol_size(int_t) == 8 + 16); /* one related entry */
	assert(!UNIQTYPE_IS_ABSTRACT(int_t));
	assert(UNIQTYPE_MAKE_PRECISE_FN(int_t) == NULL);

	/* An unbounded array (this one is in every meta-DSO's root types)
	 * is made precise from the memory bounds. */
	struct uniqtype *bytes_t = dlsym(RTLD_DEFAULT, "__uniqtype____ARR___uninterpreted_byte");
	assert(bytes_t);
	assert(UNIQTYPE_IS_ARRAY_TYPE(bytes_t));
	assert(UNIQTYPE_ARRAY_LENGTH(bytes_t) == UNIQTYPE_ARRAY_LENGTH_UNBOUNDED);
	assert(UNIQTYPE_IS_ABSTRACT(bytes_t));
	assert(UNIQTYPE_MAKE_PRECISE_FN(bytes_t) == __liballocs_make_array_precise_with_memory_bounds);

	printf("uniqtype header is %zu bytes\n", offsetof(struct uniqtype, related));
	return 0;
}
