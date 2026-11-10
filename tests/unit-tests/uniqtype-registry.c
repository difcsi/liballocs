/* Unit test for src/uniqtype-registry.c: keys are the GNU hash with the low
 * bit masked, confirmed by the name; first registered wins. Built by this
 * directory's Makefile as a single-file program. */
#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* What the registry takes from the rest of liballocs. */
void *__private_malloc(size_t n) { return malloc(n); }
void __private_free(void *p) { free(p); }

#include "../../src/uniqtype-registry.c"

static struct uniqtype a, b, c, d;

static void test_low_bit_of_hash_does_not_matter(void)
{
	/* A name whose true GNU hash is odd. */
	static char odd[64];
	for (int i = 0; ; ++i)
	{
		snprintf(odd, sizeof odd, "__uniqtype_00000001_lowbit%d", i);
		if (dl_new_hash(odd) & 1u) break;
	}
	assert((dl_new_hash(odd) & 1u) == 1u);
	/* Registered under the masked hash, as the chain array stores it... */
	assert(__liballocs_uniqtype_registry_register_hashed(odd, dl_new_hash(odd) & ~1u, &a) == &a);
	/* ... found by a lookup that hashes the string itself... */
	assert(__liballocs_uniqtype_registry_lookup(odd) == &a);
	/* ... and the first registered wins. */
	assert(__liballocs_uniqtype_registry_register(odd, &b) == &a);
}

static void test_same_hash_different_name_are_distinct(void)
{
	const char *x = "__uniqtype_aaaaaaaa_x", *y = "__uniqtype_bbbbbbbb_y";
	uint32_t hx = dl_new_hash(x) & ~1u;
	assert(__liballocs_uniqtype_registry_register_hashed(x, hx, &c) == &c);
	assert(__liballocs_uniqtype_registry_register_hashed(y, hx, &d) == &d); /* forced collision */
	assert(__liballocs_uniqtype_registry_register_hashed(x, hx, &a) == &c); /* still distinct, still first */
	assert(__liballocs_uniqtype_registry_register_hashed(y, hx, &a) == &d);
}

int main(void)
{
	test_low_bit_of_hash_does_not_matter();
	test_same_hash_different_name_are_distinct();
	printf("uniqtype-registry: ok\n");
	return 0;
}
