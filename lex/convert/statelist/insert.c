
#include <debug.h>

#include <memory/arena/realloc.h>
/*#include <memory/gneeds.h>*/

#include "compare.h"
#include "struct.h"
#include "contains.h"
#include "insert.h"

int statelist_insert(
	struct statelist* this,
	struct memory_arena* arena,
	struct nfa_node* state)
{
	int error = 0;
	size_t i;
	ENTER;
	
	dpv(this);
	dpv(state);
	
	if (!statelist_contains(this, state))
	{
		if (this->n + 1 >= this->cap)
		{
			this->cap = this->cap * 2 ?: 1;
			error = arena_realloc(arena, (void**) &this->states,
				sizeof(*this->states) * this->cap);
		}
		
		if (!error)
		{
			for (i = this->n - 1; true
				&& i + 1 >= 0 + 1
				&& compare_states(&this->states[i], &state) > 0; i--)
			{
				this->states[i + 1] = this->states[i];
			}
			
			dpv(i + 1);
			
			this->states[i + 1] = state, this->n++;
		}
	}
	
	EXIT;
	return error;
}













