// srctypes "basic" test.
//
// Builds a C++ library (libtest.so) and its meta.so (libtest-meta.so, produced
// by the srctypes uniqtype emitter reflecting testlib.hpp), then asserts that
// the emitted uniqtypes correctly describe the C++ types in testlib.hpp.
//
// Ground truth comes from the compiler's own sizeof/offsetof on the same
// testlib.hpp, so the assertions can't drift from the real ABI.
//
// The meta.so exports the canonical __uniqtype__X objects (and, by its version
// script, nothing else), so we just declare them extern and link against the
// meta.so -- no dlopen/dlsym needed. The whole transitive closure is exported,
// so even the base, array and pointer types have their own named objects.
#include <cstdio>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <climits>   // UINT_MAX, used by UNIQTYPE_HAS_KNOWN_LENGTH

#include "testlib.hpp"
#include "testlib2.hpp"
extern "C" {
#include "uniqtype-defs.h"
}

// void is a liballocs "root" type that the meta.so references but does not
// define (it lives in roottypes.c in a real build). Provide it here so the test
// links standalone; a zero-initialised uniqtype is exactly a VOID of size 0.
extern "C" struct uniqtype __uniqtype__void;
struct uniqtype __uniqtype__void = {};

// Resolved against libtest-meta.so at link time. (Codeful names sanitise the
// display name to a valid C symbol: "unsigned int" -> unsigned_int, "Box<int>"
// -> Box_int_, etc.)
extern "C" {
extern struct uniqtype __uniqtype__scalars;
extern struct uniqtype __uniqtype__point;
extern struct uniqtype __uniqtype__line;
extern struct uniqtype __uniqtype__node;
extern struct uniqtype __uniqtype__value;
extern struct uniqtype __uniqtype__buffers;
extern struct uniqtype __uniqtype__matrix;
extern struct uniqtype __uniqtype__boxes;
extern struct uniqtype __uniqtype__sample;
extern struct uniqtype __uniqtype__flags;
extern struct uniqtype __uniqtype__color;
extern struct uniqtype __uniqtype__calculator;
extern struct uniqtype __uniqtype__events;
extern struct uniqtype __uniqtype__ranged;
extern struct uniqtype __uniqtype__Box_int_;
extern struct uniqtype __uniqtype__int;
extern struct uniqtype __uniqtype__double;
extern struct uniqtype __uniqtype____PTR_node;
extern struct uniqtype __uniqtype____ARR4_int;
// from the second library source file, testlib2.cpp
extern struct uniqtype __uniqtype__rgb;
extern struct uniqtype __uniqtype__vec3;
}

#define U(x) (&__uniqtype__##x)

static void check_scalars(void)
{
    const struct uniqtype *t = U(scalars);
    assert(UNIQTYPE_KIND(t) == COMPOSITE);
    assert(UNIQTYPE_SIZE_IN_BYTES(t) == sizeof(scalars));
    assert(UNIQTYPE_COMPOSITE_MEMBER_COUNT(t) == 11);
    // spot-check a few base-type members by offset, kind and size
    assert(t->related[0].un.memb.off == offsetof(scalars, c));
    assert(UNIQTYPE_KIND(t->related[0].un.memb.ptr) == BASE);
    assert(UNIQTYPE_SIZE_IN_BYTES(t->related[0].un.memb.ptr) == sizeof(char));
    assert(t->related[4].un.memb.off == offsetof(scalars, i));
    assert(t->related[4].un.memb.ptr == U(int));      // canonical int, shared
    assert(t->related[9].un.memb.off == offsetof(scalars, d));
    assert(t->related[9].un.memb.ptr == U(double));
}

static void check_composites(void)
{
    // struct point { int x; int y; }
    const struct uniqtype *point = U(point);
    assert(UNIQTYPE_KIND(point) == COMPOSITE);
    assert(UNIQTYPE_SIZE_IN_BYTES(point) == sizeof(struct point));
    assert(UNIQTYPE_COMPOSITE_MEMBER_COUNT(point) == 2);
    assert(point->related[0].un.memb.ptr == U(int));
    assert(point->related[1].un.memb.off == offsetof(struct point, y));
    assert(point->un.composite.not_simultaneous == 0);   // a struct, not a union
    // member-names vector (related[nmemb])
    const char **pnames = UNIQTYPE_COMPOSITE_SUBOBJ_NAMES(point);
    assert(pnames && std::strcmp(pnames[0], "x") == 0 && std::strcmp(pnames[1], "y") == 0);

    // struct line { point start; point end; } -- nested, shares the point object
    const struct uniqtype *line = U(line);
    assert(UNIQTYPE_KIND(line) == COMPOSITE);
    assert(UNIQTYPE_SIZE_IN_BYTES(line) == sizeof(struct line));
    assert(line->related[0].un.memb.ptr == point);
    assert(line->related[1].un.memb.ptr == point);
    assert(line->related[1].un.memb.off == offsetof(struct line, end));

    // struct node { int val; node *next; } -- self-referential via pointer
    const struct uniqtype *node = U(node);
    assert(UNIQTYPE_KIND(node) == COMPOSITE);
    assert(node->related[0].un.memb.ptr == U(int));
    const struct uniqtype *next_t = node->related[1].un.memb.ptr;
    assert(UNIQTYPE_KIND(next_t) == ADDRESS);
    assert(next_t == U(__PTR_node));
    assert(UNIQTYPE_POINTEE_TYPE(next_t) == node);
}

static void check_union(void)
{
    // union value { int as_int; double as_double; void *as_ptr; }
    const struct uniqtype *v = U(value);
    assert(UNIQTYPE_KIND(v) == COMPOSITE);             // emitter models unions as composites
    assert(UNIQTYPE_SIZE_IN_BYTES(v) == sizeof(union value));
    assert(UNIQTYPE_COMPOSITE_MEMBER_COUNT(v) == 3);
    // all union members live at offset 0
    assert(v->related[0].un.memb.off == 0);
    assert(v->related[1].un.memb.off == 0);
    assert(v->related[2].un.memb.off == 0);
    assert(v->related[0].un.memb.ptr == U(int));
    assert(v->related[1].un.memb.ptr == U(double));
    assert(v->un.composite.not_simultaneous == 1);     // union: at most one member valid
    // as_ptr is a void *, whose pointee is the VOID root
    const struct uniqtype *vp = v->related[2].un.memb.ptr;
    assert(UNIQTYPE_KIND(vp) == ADDRESS);
    const struct uniqtype *pointee = UNIQTYPE_POINTEE_TYPE(vp);
    assert(pointee == &__uniqtype__void);
    assert(UNIQTYPE_KIND(pointee) == VOID);
}

static void check_arrays_and_pointers(void)
{
    // struct buffers { int row[4]; char name[8]; int **grid; }
    const struct uniqtype *b = U(buffers);
    assert(UNIQTYPE_KIND(b) == COMPOSITE);
    assert(UNIQTYPE_SIZE_IN_BYTES(b) == sizeof(struct buffers));

    const struct uniqtype *row = b->related[0].un.memb.ptr;
    assert(UNIQTYPE_IS_ARRAY_TYPE(row));
    assert(UNIQTYPE_ARRAY_LENGTH(row) == 4);
    assert(UNIQTYPE_ARRAY_ELEMENT_TYPE(row) == U(int));
    assert(row == U(__ARR4_int));                      // canonical array object

    const struct uniqtype *name = b->related[1].un.memb.ptr;
    assert(UNIQTYPE_IS_ARRAY_TYPE(name));
    assert(UNIQTYPE_ARRAY_LENGTH(name) == 8);
    assert(UNIQTYPE_SIZE_IN_BYTES(UNIQTYPE_ARRAY_ELEMENT_TYPE(name)) == sizeof(char));

    // int ** : pointer-to-pointer-to-int, indir_level 2
    const struct uniqtype *grid = b->related[2].un.memb.ptr;
    assert(UNIQTYPE_KIND(grid) == ADDRESS);
    assert(grid->un.address.indir_level == 2);
    const struct uniqtype *grid1 = UNIQTYPE_POINTEE_TYPE(grid);
    assert(UNIQTYPE_KIND(grid1) == ADDRESS);
    assert(grid1->un.address.indir_level == 1);
    assert(UNIQTYPE_POINTEE_TYPE(grid1) == U(int));
    // related[1] of a multi-level pointer is the ultimate (non-pointer) pointee
    assert(UNIQTYPE_ULTIMATE_POINTEE_TYPE(grid) == U(int));

    // struct matrix { char (*rows)[]; } : pointer to an UNBOUNDED array
    const struct uniqtype *m = U(matrix);
    const struct uniqtype *rows = m->related[0].un.memb.ptr;
    assert(UNIQTYPE_KIND(rows) == ADDRESS);
    const struct uniqtype *arr = UNIQTYPE_POINTEE_TYPE(rows);
    assert(UNIQTYPE_IS_ARRAY_TYPE(arr));
    assert((unsigned) UNIQTYPE_ARRAY_LENGTH(arr) == UNIQTYPE_ARRAY_LENGTH_UNBOUNDED);
    assert(!UNIQTYPE_HAS_KNOWN_LENGTH(arr));             // pos_maxoff == UINT_MAX
    assert(UNIQTYPE_SIZE_IN_BYTES(UNIQTYPE_ARRAY_ELEMENT_TYPE(arr)) == sizeof(char));
}

static void check_templates(void)
{
    // struct boxes { Box<int> bi; Box<double> bd; }
    const struct uniqtype *boxes = U(boxes);
    assert(UNIQTYPE_KIND(boxes) == COMPOSITE);
    assert(UNIQTYPE_COMPOSITE_MEMBER_COUNT(boxes) == 2);

    const struct uniqtype *bi = boxes->related[0].un.memb.ptr;
    assert(bi == U(Box_int_));                          // Box<int> by canonical name
    assert(UNIQTYPE_KIND(bi) == COMPOSITE);
    assert(UNIQTYPE_SIZE_IN_BYTES(bi) == sizeof(Box<int>));
    assert(UNIQTYPE_COMPOSITE_MEMBER_COUNT(bi) == 1);
    assert(bi->related[0].un.memb.ptr == U(int));       // Box<int>::value : int

    const struct uniqtype *bd = boxes->related[1].un.memb.ptr;
    assert(UNIQTYPE_KIND(bd) == COMPOSITE);
    assert(bd->related[0].un.memb.ptr == U(double));    // Box<double>::value : double
    assert(bd != bi);                                   // distinct instantiations
}

static void check_mixed(void)
{
    // struct sample { value v; color tag; }
    const struct uniqtype *s = U(sample);
    assert(UNIQTYPE_KIND(s) == COMPOSITE);
    assert(UNIQTYPE_COMPOSITE_MEMBER_COUNT(s) == 2);
    assert(s->related[0].un.memb.ptr == U(value));      // shares the union object

    // enum color { RED, GREEN, BLUE } : a real ENUMERATION uniqtype
    const struct uniqtype *tag = s->related[1].un.memb.ptr;
    assert(tag == U(color));
    assert(UNIQTYPE_KIND(tag) == ENUMERATION);
    assert(UNIQTYPE_SIZE_IN_BYTES(tag) == sizeof(color));
    assert(tag->un.enumeration.nenum == 3);
    // related[0] is the underlying base type; related[1..] are the values
    const struct uniqtype *base = UNIQTYPE_ENUM_BASE_TYPE(tag);
    assert(UNIQTYPE_KIND(base) == BASE);
    assert(UNIQTYPE_SIZE_IN_BYTES(base) == sizeof(color));
    assert(tag->related[1].un.enumerator.val == RED);
    assert(tag->related[2].un.enumerator.val == GREEN);
    assert(tag->related[3].un.enumerator.val == BLUE);
}

static void check_bitfields(void)
{
    // struct flags { unsigned a:3; unsigned b:5; int x; }
    const struct uniqtype *f = U(flags);
    assert(UNIQTYPE_KIND(f) == COMPOSITE);
    assert(UNIQTYPE_COMPOSITE_MEMBER_COUNT(f) == 3);

    // a : 3 bits at bit offset 0 -- a member-local bit-granularity base type
    const struct uniqtype *a = f->related[0].un.memb.ptr;
    assert(UNIQTYPE_KIND(a) == BASE);
    assert(UNIQTYPE_IS_BIT_GRANULARITY_BASE_TYPE(a));
    assert(UNIQTYPE_BASE_TYPE_BIT_SIZE(a) == 3);
    assert(UNIQTYPE_BASE_TYPE_BIT_OFFSET(a) == 0);

    // b : 5 bits at bit offset 3
    const struct uniqtype *b = f->related[1].un.memb.ptr;
    assert(UNIQTYPE_IS_BIT_GRANULARITY_BASE_TYPE(b));
    assert(UNIQTYPE_BASE_TYPE_BIT_SIZE(b) == 5);
    assert(UNIQTYPE_BASE_TYPE_BIT_OFFSET(b) == 3);
    assert(a != b);                                       // distinct synthetic types

    // x : an ordinary int (the canonical one), not bit-granularity
    assert(f->related[2].un.memb.off == offsetof(struct flags, x));
    assert(f->related[2].un.memb.ptr == U(int));
    assert(!UNIQTYPE_IS_BIT_GRANULARITY_BASE_TYPE(U(int)));
}

static void check_function(void)
{
    // struct calculator { binop op; value last; color mode; }, with
    // binop = int (*)(int, int)
    const struct uniqtype *calc = U(calculator);
    assert(UNIQTYPE_KIND(calc) == COMPOSITE);
    assert(calc->related[0].un.memb.off == offsetof(struct calculator, op));

    // op is a pointer to a function type
    const struct uniqtype *op = calc->related[0].un.memb.ptr;
    assert(UNIQTYPE_KIND(op) == ADDRESS);
    const struct uniqtype *fn = UNIQTYPE_POINTEE_TYPE(op);
    assert(UNIQTYPE_KIND(fn) == SUBPROGRAM);
    assert(UNIQTYPE_SUBPROGRAM_ARG_COUNT(fn) == 2);
    assert(fn->un.subprogram.nret == 1);
    assert(fn->un.subprogram.is_va == 0);
    // related[0] = return type, related[1..] = argument types -- all int here
    assert(fn->related[0].un.t.ptr == U(int));
    assert(fn->related[1].un.t.ptr == U(int));
    assert(fn->related[2].un.t.ptr == U(int));
    assert(fn->un.subprogram.is_va == 0);

    // struct events { int (*report)(int, ...); } : a *variadic* SUBPROGRAM
    const struct uniqtype *ev = U(events);
    const struct uniqtype *rep = ev->related[0].un.memb.ptr;
    assert(UNIQTYPE_KIND(rep) == ADDRESS);
    const struct uniqtype *vfn = UNIQTYPE_POINTEE_TYPE(rep);
    assert(UNIQTYPE_KIND(vfn) == SUBPROGRAM);
    assert(vfn->un.subprogram.is_va == 1);
    assert(UNIQTYPE_SUBPROGRAM_ARG_COUNT(vfn) == 1);   // the one fixed 'int' arg
    assert(vfn->related[0].un.t.ptr == U(int));        // return int
    assert(vfn->related[1].un.t.ptr == U(int));        // fixed arg int
}

static void check_subrange(void)
{
    // struct ranged { srctypes::subrange<int, 1, 31> day; } : the opt-in SUBRANGE
    const struct uniqtype *r = U(ranged);
    assert(UNIQTYPE_KIND(r) == COMPOSITE);
    const struct uniqtype *day = r->related[0].un.memb.ptr;
    assert(UNIQTYPE_KIND(day) == SUBRANGE);
    assert(day->un.subrange.min == 1);
    assert(day->un.subrange.max == 31);
    assert(UNIQTYPE_SIZE_IN_BYTES(day) == sizeof(int));
    assert(day->related[0].un.t.ptr == U(int));        // host type
}

static void check_second_file(void)
{
    // Types declared in the *other* source file (testlib2.hpp/.cpp) are covered
    // too: gen-meta was given both library sources.
    const struct uniqtype *rgb = U(rgb);
    assert(UNIQTYPE_KIND(rgb) == COMPOSITE);
    assert(UNIQTYPE_SIZE_IN_BYTES(rgb) == sizeof(struct rgb));
    assert(UNIQTYPE_COMPOSITE_MEMBER_COUNT(rgb) == 3);

    const struct uniqtype *vec3 = U(vec3);
    assert(UNIQTYPE_KIND(vec3) == COMPOSITE);
    assert(UNIQTYPE_COMPOSITE_MEMBER_COUNT(vec3) == 3);
    assert(vec3->related[0].un.memb.ptr == U(double));
}

int main(void)
{
    check_scalars();
    check_composites();
    check_union();
    check_arrays_and_pointers();
    check_templates();
    check_mixed();
    check_bitfields();
    check_function();
    check_subrange();
    check_second_file();
    std::printf("srctypes basic test: all uniqtypes correct\n");
    return 0;
}
