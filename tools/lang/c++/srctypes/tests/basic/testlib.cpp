
#include "testlib.hpp"

scalars the_scalars = {};
point   the_origin  = { 0, 0 };
line    the_axis    = { { 0, 0 }, { 1, 0 } };
node    the_head    = { 42, nullptr };
value   the_value   = { 7 };
color   the_color   = GREEN;
buffers the_buffers = {};
matrix  the_matrix  = { nullptr };
boxes   the_boxes   = { { 1 }, { 2.0 } };
sample  the_sample  = { { 3 }, BLUE };
flags   the_flags   = {};

int add(int a, int b) { return a + b; }
double scale(double x, int by) { return x * by; }
int report(int n, ...) { return n; }

calculator the_calc   = { add, { 0 }, RED };
events     the_events = { report };
ranged     the_ranged = { { 1 } };

template int identity<int>(int);
