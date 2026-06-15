#ifndef SRCTYPES_BASIC_TESTLIB_HPP
#define SRCTYPES_BASIC_TESTLIB_HPP

#include "srctypes_subrange.hpp"

// --- all the base types, in one composite ---
struct scalars {
    char           c;
    signed char    sc;
    unsigned char  uc;
    short          s;
    int            i;
    long           l;
    unsigned       u;
    unsigned long  ul;
    float          f;
    double         d;
    bool           b;
};

// --- plain composite, and a composite nested inside another ---
struct point { int x; int y; };
struct line  { point start; point end; };

// --- self-referential composite, via a pointer member ---
struct node { int val; node *next; };

// --- union ---
union value { int as_int; double as_double; void *as_ptr; };

// --- enum (the emitter reduces it to its underlying base type) ---
enum color { RED, GREEN, BLUE };

// --- arrays and multi-level pointers ---
struct buffers {
    int    row[4];
    char   name[8];
    int  **grid;       // pointer-to-pointer
};

// --- pointer to an array of unknown bound: an UNBOUNDED array uniqtype ---
struct matrix {
    char (*rows)[];
};

// --- class template, instantiated at two argument types ---
template <typename T> struct Box { T value; };
struct boxes {
    Box<int>    bi;
    Box<double> bd;
};

// --- a small aggregate mixing a union and an enum member ---
struct sample {
    value v;
    color tag;
};

// --- bit-fields: each gets a member-local bit-granularity base type ---
struct flags {
    unsigned a : 3;
    unsigned b : 5;
    int       x;
};

// --- function pointers: a struct member of function-pointer type makes a
//     SUBPROGRAM uniqtype reachable ---
typedef int (*binop)(int, int);
struct calculator {
    binop op;            // pointer to int(int,int)
    value last;
    color mode;
};

// --- a variadic function pointer (is_va SUBPROGRAM) ---
typedef int (*reporter)(int, ...);
struct events {
    reporter report;
};

// --- opt-in SUBRANGE: a host int constrained to [1, 31] ---
struct ranged {
    srctypes::subrange<int, 1, 31> day;
};

// --- free functions and a function template: real symbols in the library ---
int    add(int a, int b);
double scale(double x, int by);
template <typename T> T identity(T v) { return v; }

#endif
