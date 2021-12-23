
#include <debug.h>

#include <memory/arena/malloc.h>

#include "struct.h"
#include "new.h"

int new_nfa_node(
	struct nfa_node** out,
	struct memory_arena* arena)
{
	int error = 0;
	ENTER;
	
	struct nfa_node* this = NULL;
	
	error = arena_malloc(arena, (void**) &this, sizeof(*this));
	
	if (!error)
	{
		this->transitions.data = NULL;
		this->transitions.n = 0;
		this->transitions.cap = 0;
		
		this->default_transition = NULL;
		
		this->lambda_transitions.data = NULL;
		this->lambda_transitions.n = 0;
		this->lambda_transitions.cap = 0;
		
		this->rule = NULL;
		
		*out = this;
	}
	
	EXIT;
	return error;
}

