# srctypes — a reflection-based uniqtype emitter

`srctypes` builds liballocs **uniqtypes** straight from C/C++ source, using C++26
static reflection (`<meta>`, P2996) instead of DWARF.

## Build & test

```sh
make check          # syntax-check the sources and run the test suite
make -C src check   # check whether embedded code is compliable.
make -C tests/basic check
```

To run the driver by hand on a library's sources (after `make -C src`):

```sh
SRCTYPES_CXXFLAGS='-std=c++26 -freflection -I/path/to/liballocstool/include' \
    src/gen-meta -o libmylib-meta.so a.cpp b.cpp ...  # build the meta-DSO
src/gen-meta --dump a.cpp b.cpp                        # or just print the meta TU
```
