THIS_DIR := $(patsubst %/,%,$(dir $(realpath $(lastword $(MAKEFILE_LIST)))))

SRCTYPES_SRC_INCLUDE := 1
include $(THIS_DIR)/../../src/Makefile


LIB_SRCS := testlib.cpp testlib2.cpp
LIB_HDRS := testlib.hpp testlib2.hpp

.DEFAULT_GOAL := check
.PHONY: check clean
check: main libtest.so
	./main


libtest.so: $(LIB_SRCS) $(LIB_HDRS)
	$(CXX_HOST) $(CXXFLAGS_HOST) -I$(SRCTYPES_SRC_DIR) -fPIC -shared -o $@ $(LIB_SRCS)

libtest-meta.so: $(LIB_SRCS) $(LIB_HDRS) $(GEN_META)
	SRCTYPES_CXX='$(CXX)' SRCTYPES_CXXFLAGS='$(CXXFLAGS_META) -I$(UNIQTYPE_INC) -I$(SRCTYPES_SRC_DIR)' \
	    $(GEN_META) -o $@ $(LIB_SRCS)

main: main.cpp $(LIB_HDRS) libtest-meta.so
	$(CXX_HOST) $(CXXFLAGS_HOST) -I$(UNIQTYPE_INC) -I. -I$(SRCTYPES_SRC_DIR) main.cpp libtest-meta.so -o $@ -Wl,-rpath,'$$ORIGIN'

clean:
	rm -f main libtest.so libtest-meta.so
