
#include <debug.h>

/*#include <memory/arena/realloc.h>*/

#include "struct.h"
#include "set_default_branch.h"

int nfa_node_set_default_branch(
	struct nfa_node* this,
	struct memory_arena* arena,
	struct nfa_node* branch)
{
	int error = 0;
	ENTER;
	
	assert(!this->default_transition);
	
	this->default_transition = branch;
	
	EXIT;
	return error;
}

