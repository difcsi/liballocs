/* Linked into every -meta.so by tools/Makefile.meta, so that liballocs can
 * tell which metadata ABI a meta-DSO was generated for. See include/meta-abi.h. */
#include "meta-abi.h"

#ifdef LIBALLOCS_META_ABI_VERSION_OVERRIDE /* lets a test build a deliberately stale meta-DSO */
#undef LIBALLOCS_META_ABI_VERSION
#define LIBALLOCS_META_ABI_VERSION LIBALLOCS_META_ABI_VERSION_OVERRIDE
#endif

const unsigned __liballocs_meta_abi_version = LIBALLOCS_META_ABI_VERSION;
