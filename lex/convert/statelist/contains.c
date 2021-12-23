
#include <debug.h>

#include "struct.h"
#include "compare.h"
#include "contains.h"

bool statelist_contains(struct statelist* this, struct nfa_node* nfa_node)
{
	bool retval;
	ENTER;
	
	dpv(this);
	dpv(nfa_node);
	
	retval = !!bsearch(&nfa_node, this->states, this->n, sizeof(*this->states), compare_states);
	
	dpvb(retval);
	
	EXIT;
	return retval;
}
