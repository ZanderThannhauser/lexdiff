
#include <debug.h>

#include <memory/arena/realloc.h>

#include "struct.h"
#include "add_default_branch.h"

int nfa_node_add_default_branch(
	struct nfa_node* this,
	struct arena* arena,
	struct nfa_node* branch)
{
	int error = 0;
	ENTER;
	
	if (this->default_branches.n + 1 >= this->default_branches.cap)
	{
		this->default_branches.cap = this->default_branches.cap * 2 ?: 1;
		
		error = arena_realloc(
			arena,
			(void**) &this->default_branches.data,
			sizeof(*this->default_branches.data) * 
				this->default_branches.cap);
	}
	
	if (!error)
	{
		this->default_branches.data[this->default_branches.n++] = branch;
	}
	
	EXIT;
	return error;
}

