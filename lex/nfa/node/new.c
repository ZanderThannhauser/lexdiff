
#include <debug.h>

#include <memory/arena/malloc.h>

#include "struct.h"
#include "new.h"

int new_nfa_node(
	struct nfa_node** out,
	struct arena* arena)
{
	int error = 0;
	ENTER;
	
	struct nfa_node* this = NULL;
	
	error = arena_malloc(arena, (void**) &this, sizeof(*this));
	
	if (!error)
	{
		this->normal_branches.data = NULL;
		this->normal_branches.n = 0;
		this->normal_branches.cap = 0;
		
		this->default_branches.data = NULL;
		this->default_branches.n = 0;
		this->default_branches.cap = 0;
		
		this->lambda_branches.data = NULL;
		this->lambda_branches.n = 0;
		this->lambda_branches.cap = 0;
		
		*out = this;
	}
	
	EXIT;
	return error;
}

