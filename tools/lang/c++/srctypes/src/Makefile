SRCTYPES_SRC_DIR := $(patsubst %/,%,$(dir $(realpath $(lastword $(MAKEFILE_LIST)))))
SRCTYPES_DIR     := $(realpath $(SRCTYPES_SRC_DIR)/..)
SRCTYPES_ROOT    := $(realpath $(SRCTYPES_DIR)/../../../..)


EMITTER      := $(SRCTYPES_SRC_DIR)/uniqtype_emit.hpp
DISCOVER_SRC := $(SRCTYPES_SRC_DIR)/discover.cpp
GEN_META     := $(SRCTYPES_SRC_DIR)/gen-meta
UNIQTYPE_INC := $(SRCTYPES_ROOT)/contrib/liballocstool/include


ifeq ($(origin CXX),default)
CXX := g++-16
endif
CXXFLAGS_META ?= -std=c++26 -freflection -fPIC -O0 -g

CXX_HOST      ?= c++
CXXFLAGS_HOST ?= -std=c++17 -O2

$(GEN_META): $(SRCTYPES_SRC_DIR)/gen_meta.cpp $(EMITTER) $(DISCOVER_SRC) \
             $(SRCTYPES_SRC_DIR)/srctypes_codeful.hpp $(SRCTYPES_SRC_DIR)/srctypes_subrange.hpp \
             $(SRCTYPES_SRC_DIR)/meta.map
	$(CXX_HOST) $(CXXFLAGS_HOST) $< -o $@

ifndef SRCTYPES_SRC_INCLUDE


.DEFAULT_GOAL := check
.PHONY: check clean

# End-to-end smoke test: better find out that the emitter doesn't compile eagerly
check: $(GEN_META)
	@printf 'struct _srctypes_probe { int a; struct _srctypes_probe *self; };\n' > .probe.cpp
	SRCTYPES_CXX='$(CXX)' SRCTYPES_CXXFLAGS='$(CXXFLAGS_META) -I$(UNIQTYPE_INC) -I$(SRCTYPES_SRC_DIR)' \
	    $(GEN_META) -o .probe-meta.so .probe.cpp
	@rm -f .probe.cpp .probe-meta.so
	@echo "srctypes sources OK"

clean:
	rm -f .probe.cpp .probe-meta.so $(GEN_META)

endif
