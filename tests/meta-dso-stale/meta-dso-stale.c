#include <assert.h>
#include "allocs.h"
#include "allocmeta.h"

/* Our meta-DSO is relinked by mk.inc claiming metadata ABI version 0,
 * so liballocs must decline to use it (see include/meta-abi.h). */
int main(void)
{
	void *file_metadata = __liballocs_get_specific_by_allocator(main,
		&__static_file_allocator, NULL);
	assert(file_metadata);
	struct allocs_file_metadata *meta = file_metadata;
	assert(!meta->meta_obj_handle);
	return 0;
}
