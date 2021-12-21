
#include <debug.h>

#include "prettyprint.h"
/*#include "header.h"*/
#include "get_size.h"
#include "malloc.h"
#include "dealloc.h"
#include "realloc.h"

int arena_realloc(
	struct arena* this,
	void** ptr, size_t size)
{
	int error = 0;
	ENTER;
	
	dpv(*ptr);
	dpv(size);
	
	if (!*ptr)
		error = arena_malloc(this, ptr, size);
	else if (!size)
		arena_dealloc(this, *ptr);
	else
	{
		void *old = *ptr, *new = NULL;
		
		error = arena_malloc(this, &new, size);
		
		if (!error)
		{
			memcpy(new, old, arena_get_size(old));
			
			arena_dealloc(this, old);
			
			arena_prettyprint(this);
			TODO;
		}
	}
	
	EXIT;
	return error;
}

