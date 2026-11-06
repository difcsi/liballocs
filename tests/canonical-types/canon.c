#include <stdlib.h>

/* Both this library and the program define struct pair, so both their
 * meta-DSOs define its uniqtype. Everything this library's metadata says
 * about pairs and ints must resolve to the canonical uniqtypes. */
struct pair { int first; int second; };

struct pair canon_static_pair = { 1, 2 };
int *canon_static_ptr = &canon_static_pair.first;

struct pair *new_pair(void)
{
	return malloc(sizeof (struct pair));
}
