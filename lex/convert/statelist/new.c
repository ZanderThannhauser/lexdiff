
#include <debug.h>

#include <memory/arena/malloc.h>

#include "struct.h"
#include "new.h"

int new_statelist(
	struct statelist** new,
	struct memory_arena* arena)
{
	int error = 0;
	struct statelist* this = NULL;
	ENTER;
	
	error = arena_malloc(arena, (void**) &this, sizeof(*this));
	
	if (!error)
	{
		this->states = NULL;
		this->n = 0;
		this->cap = 0;
		
		*new = this;
	}
	
	EXIT;
	return error;
}

