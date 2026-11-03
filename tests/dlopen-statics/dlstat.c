/* This library is dlopen'd by the program, not loaded at startup. Its
 * segments are defined before its meta-DSO is loaded, so liballocs must
 * set up its metavectors again once that has happened. */
struct pair { int first; int second; };

struct pair dlstat_pair = { 1, 2 };
static int dlstat_local = 5;

/* Accessors, so the program need not name the variables itself. */
struct pair *dlstat_pair_addr(void) { return &dlstat_pair; }
int *dlstat_local_addr(void) { return &dlstat_local; }
