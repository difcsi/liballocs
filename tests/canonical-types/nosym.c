/* A static described by DWARF but by no ELF symbol: mk.inc strips its
 * symbol from this object file. The meta-DSO must then describe it with an
 * "extrasym", and its metavector record must say so. */
static double canon_nosym = 7.0;

double *canon_nosym_addr(void) { return &canon_nosym; }
