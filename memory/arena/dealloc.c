
#include <debug.h>

#include "header.h"
#include "dealloc.h"

void arena_dealloc(
	struct arena* this,
	void* ptr)
{
	ENTER;
	
	dpv(ptr);
	
	if (ptr)
	{
		struct arena_header* header = ptr - sizeof(*header);
		
		struct arena_footer* footer =
			((void*) header) + header->size - sizeof(*footer);
		
		TODO;
	}
	
	EXIT;
}

