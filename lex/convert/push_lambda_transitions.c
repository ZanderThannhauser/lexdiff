
#include <debug.h>

#include <lex/nfa_node/struct.h>

#include "statelist/struct.h"
#include "statelist/insert.h"
#include "statelist/contains.h"

#include "push_lambda_transitions.h"

int push_lambda_transitions(
	struct statelist* statelist,
	struct memory_arena* arena)
{
	int error = 0;
	ENTER;
	
	bool again;
	size_t i, n;
	struct nfa_node *state1, *state2;
	size_t j, m;
	
	do for (again = false, i = 0, n = statelist->n; !error && i < n; i++)
	{
		dpv(i);
		
		state1 = statelist->states[i];
		
		dpv(state1);
		
		for (j = 0, m = state1->lambda_transitions.n; !error && j < m; j++)
		{
			dpv(j);
			
			dpv(state1->lambda_transitions.data);
			
			state2 = state1->lambda_transitions.data[j];
			
			dpv(state2);
			
			if (!statelist_contains(statelist, state2))
			{
				verprintf("adding %p\n", state2);
				error = statelist_insert(statelist, arena, state2);
				again = true;
			}
		}
	} while (!error && again);
	
	EXIT;
	return error;
}





















