
#include <debug.h>

#include "header.h"
#include "footer.h"
#include "get_size.h"

size_t arena_get_size(void* ptr)
{
	ENTER;
	
	dpv(ptr);
	
	assert(ptr);
	
	struct arena_header* header = ptr - sizeof(*header);
	
	size_t block_size = header->size;
	
	dpv(block_size);
	
	size_t user_size = block_size - sizeof(*header) - sizeof(struct arena_footer);
	
	dpv(user_size);
	
	EXIT;
	return user_size;
}

