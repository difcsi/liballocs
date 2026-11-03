THIS_MAKEFILE := $(lastword $(MAKEFILE_LIST))
here := $(dir $(realpath $(THIS_MAKEFILE)))

# Build as usual (allocscc generates the meta-DSO), then relink the meta-DSO
# claiming a bogus metadata ABI version; liballocs must decline to use it.
meta-dso-stale: meta-dso-stale.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< $(LOADLIBES) $(LDLIBS) -o $@
	$(MAKE) -f $(LIBALLOCS)/tools/Makefile.meta META_CC=cc \
	    META_CFLAGS="$(META_CFLAGS) -DLIBALLOCS_META_ABI_VERSION_OVERRIDE=0" \
	    -W $(LIBALLOCS)/tools/meta-abi.c $(META_BASE)$(here)meta-dso-stale-meta.so
