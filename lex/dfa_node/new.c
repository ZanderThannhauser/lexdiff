
#include <debug.h>

#include <memory/arena/malloc.h>

#include "struct.h"
#include "new.h"

int new_dfa_node(
	struct dfa_node** new,
	struct memory_arena* arena)
{
	int error = 0;
	struct dfa_node* this;
	ENTER;
	
	error = arena_malloc(arena, (void**) &this, sizeof(*this));
	
	if (!error)
	{
		this->transitions.data = NULL;
		this->transitions.cap = 0;
		this->transitions.n = 0;
		
		this->default_transition = NULL;
		
		this->loop_guard = false;
		
		this->rule = NULL;
		
		*new = this;
	}
	
	EXIT;
	return error;
}

