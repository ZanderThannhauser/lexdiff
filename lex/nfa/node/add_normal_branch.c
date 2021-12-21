
#include <debug.h>

#include <memory/arena/realloc.h>

#include "struct.h"
#include "add_normal_branch.h"

int nfa_node_add_normal_branch(
	struct nfa_node* this,
	struct arena* arena,
	wchar_t codepoint,
	struct nfa_node* node)
{
	int error = 0;
	ENTER;
	
	dpv(codepoint);
	
	if (this->normal_branches.n + 1 >= this->normal_branches.cap)
	{
		this->normal_branches.cap = this->normal_branches.cap * 2 ?: 1;
		
		error = arena_realloc(
			arena,
			(void**) &this->normal_branches.data,
			sizeof(*this->normal_branches.data) * 
				this->normal_branches.cap);
	}
	
	if (!error)
	{
		this->normal_branches.data[this->normal_branches.n].codepoint = codepoint;
		this->normal_branches.data[this->normal_branches.n].node= node;
		
		this->normal_branches.n++;
	}
	
	EXIT;
	return error;
}

