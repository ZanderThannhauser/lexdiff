
#include <debug.h>

#include <memory/arena/realloc.h>

#include "struct.h"
#include "add_lambda_branch.h"

int nfa_node_add_lambda_branch(
	struct nfa_node* this,
	struct memory_arena* arena,
	struct nfa_node* node)
{
	int error = 0;
	ENTER;
	
	dpv(this->lambda_transitions.data);
	
	if (this->lambda_transitions.n + 1 >= this->lambda_transitions.cap)
	{
		this->lambda_transitions.cap = this->lambda_transitions.cap * 2 ?: 1;
		
		error = arena_realloc(
			arena,
			(void**) &this->lambda_transitions.data,
			sizeof(*this->lambda_transitions.data) * 
				this->lambda_transitions.cap);
	}
	
	if (!error)
	{
		dpv(node);
		
		dpv(this->lambda_transitions.data);
		
		this->lambda_transitions.data[this->lambda_transitions.n++] = node;
		
		dpv(this->lambda_transitions.n);
	}
	
	EXIT;
	return error;
}

