#include <stdlib.h>

/* Both this library and the program define struct pair, so both their
 * meta-DSOs define its uniqtype. Everything this library's metadata says
 * about pairs and ints must resolve to the canonical uniqtypes. */
struct pair { int first; int second; };

struct pair canon_static_pair = { 1, 2 };
int *canon_static_ptr = &canon_static_pair.first;
static int canon_local = 5;

/* The program reaches our statics through these, so that they stay here
 * rather than getting copy relocations into its own data segment. */
struct pair *canon_pair_addr(void) { return &canon_static_pair; }
int **canon_ptr_addr(void) { return &canon_static_ptr; }
int *canon_local_addr(void) { return &canon_local; }

struct pair *new_pair(void)
{
	return malloc(sizeof (struct pair));
}
