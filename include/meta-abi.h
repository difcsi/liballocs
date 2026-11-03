#ifndef LIBALLOCS_META_ABI_H_
#define LIBALLOCS_META_ABI_H_

/* Every -meta.so carries this number as the object __liballocs_meta_abi_version
 * (from tools/meta-abi.c, linked in by tools/Makefile.meta). liballocs declines
 * to use a meta-DSO whose number differs from the one it was built with, or
 * which has none (i.e. predates versioning), rather than misread it.
 *
 * Bump it whenever the layout of anything in a meta-DSO changes: uniqtypes
 * (contrib/liballocstool's uniqtype-defs.h and what its generator emits),
 * allocsites, the metavector, frame info, extrasyms. */
#define LIBALLOCS_META_ABI_VERSION 2
#define LIBALLOCS_META_ABI_VERSION_SYMBOL "__liballocs_meta_abi_version"

#endif
