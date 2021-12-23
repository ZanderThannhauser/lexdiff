
#include <avl/avl.h>

#include <debug.h>

#include <enums/error.h>

#include <structs/user_rule.h>

#include <lex/nfa_node/struct.h>
#include <lex/dfa_node/struct.h>
#include <lex/dfa_node/new.h>
#include <lex/dfa_node/add_transition.h>

#include <memory/arena/malloc.h>
#include <memory/arena/dealloc.h>
/*#include <memory/grealloc.h>*/
/*#include <memory/gneeds.h>*/
/*#include <memory/ginc.h>*/
/*#include <memory/delete.h>*/

#include "statelist/struct.h"
#include "statelist/new.h"
#include "statelist/insert.h"

#include "bundle.h"
#include "free_bundle.h"
#include "push_lambda_transitions.h"
#include "combine_states.h"

int combine_states(
	struct avl_tree_t* tree,
	struct statelist* statelist,
	struct memory_arena* throwaway_arena,
	struct memory_arena* dfa_arena,
	struct dfa_node** retval)
{
	int error = 0;
	avl_node_t* node;
	ENTER;
	
	dpv(statelist->n);
	
	// have we done this list before?
	if ((node = avl_search(tree, &statelist)))
	{
		struct bundle* bundle;
		// return reference to already done state
		
		verprintf("already done!\n");
		
		bundle = node->item;
		
		*retval = bundle->combined;
	}
	else
	{
		size_t i, n;
/*		struct state** moving;*/
		struct dfa_node* combined_state = NULL;
		
		// create new state, create new bundle, push to tree
		{
			struct bundle* bundle = NULL;
			
			error = 0
				?: new_dfa_node(&combined_state, dfa_arena)
				?: arena_malloc(throwaway_arena, (void**) &bundle, sizeof(*bundle));
			
			if (!error)
			{
				bundle->statelist = statelist;
				bundle->combined = combined_state;
				
				if (!(node = avl_insert(tree, bundle)))
					error = e_out_of_memory;
			}
			
			if (error)
				free_bundle(bundle);
		}
		
		// set this as accepting if any states in list are accepting
		if (!error)
		{
			struct user_rule *rule = NULL, *elerule;
			
			for (i = 0, n = statelist->n; i < n; i++)
				if ((elerule = statelist->states[i]->rule) &&
					(!rule || elerule->id < rule->id))
				{
					rule = elerule;
				}
			
			combined_state->rule = rule;
			
			dpv(combined_state->rule);
		}
		
		struct nfa_node* state;
		struct nfa_node_branch* transition;
		
		// create transition iterators for each:
		struct iterator {
			struct nfa_node_branch *moving, *end;
		} (*iterators)[statelist->n] = NULL, *iterator;
		
		size_t iterators_n = 0;
		
		if (!error)
			error = arena_malloc(throwaway_arena, (void**) &iterators,
				sizeof(*iterators));
			
		if (!error)
		{
			for (i = 0, n = statelist->n; i < n; i++)
			{
				state = statelist->states[i];
				
				if (state->transitions.n)
				{
					iterators_n++;
				}
				
				(*iterators)[i] = (struct iterator) {
					.moving = state->transitions.data,
					.end = state->transitions.data + state->transitions.n
				};
			}
		}
		
/*		size_t N;*/
/*		size_t j, m;*/
		unsigned lowest;
		struct dfa_node* transition_combined_state;
		struct statelist* transition_statelist;
		// until there are no more transitions:
		for (size_t N = iterators_n; !error && N > 0; )
		{
			HERE;
			dpv(N);
			
			transition_statelist = NULL;
			
			dpv(statelist->n);
			HERE;
			
			// what's the next lowest transition value in all states in list?
			for (lowest = -1, i = 0, n = statelist->n; i < n; i++)
			{
				iterator = &((*iterators)[i]);
				
				dpv(iterator->moving);
				dpv(iterator->end);
				
				if (iterator->moving < iterator->end)
				{
					transition = (*iterators)[i].moving;
					
					dpv(transition->value);
					
					if (transition->value < lowest)
					{
						lowest = transition->value;
					}
				}
			}
			
			dpv(lowest);
			
			error = new_statelist(&transition_statelist, throwaway_arena);
			
			// make a list of all the new places we would be,
			for (i = 0, n = statelist->n; !error && i < n; i++)
			{
				iterator = &((*iterators)[i]);
				
				if (iterator->moving < iterator->end &&
					iterator->moving[0].value == lowest)
				{
					error = statelist_insert(transition_statelist,
						throwaway_arena, iterator->moving[0].to);
					
					if (++iterator->moving == iterator->end)
					{
						N--;
					}
				}
				else if ((state = statelist->states[i]->default_transition))
				{
					error = statelist_insert(transition_statelist,
						throwaway_arena, state);
				}
			}
			
			dpv(transition_statelist->n);
			
			// if nonempty list:
			if (!error && transition_statelist->n > 0)
			{
				transition_combined_state = NULL;
				
				dpv(transition_statelist->n);
				
				error = 0
					// add lambda transitions to elements in list
					?: push_lambda_transitions(transition_statelist, throwaway_arena)
					// call convert(on that new list)
					?: combine_states(tree, transition_statelist,
						throwaway_arena, dfa_arena, &transition_combined_state)
					// add new transition to new state to go to returned state
					?: dfa_node_add_transition(combined_state, dfa_arena,
						lowest, transition_combined_state);
			}
		}
		
		// call convert on the list of all default-transitions, set result to
		// new state's default transition
		if (!error)
		{
			transition_statelist = NULL;
			
			error = new_statelist(&transition_statelist, throwaway_arena);
			
			for (i = 0, n = statelist->n; !error && i < n; i++)
				if ((state = statelist->states[i]->default_transition))
					error = statelist_insert(transition_statelist, throwaway_arena, state);
			
			
			// if nonempty list:
			if (!error && transition_statelist->n > 0)
			{
				transition_combined_state = NULL;
				
				dpv(transition_statelist->n);
				
				// add lambda transitions to elements in list
				error = push_lambda_transitions(transition_statelist, throwaway_arena);
				
				dpv(transition_statelist->n);
				
				// call convert(on that new list)
				if (!error)
					error = combine_states(tree, transition_statelist,
						throwaway_arena, dfa_arena, &transition_combined_state);
				
				// add new transition to new state to go to returned state
				if (!error)
				{
					combined_state->default_transition = transition_combined_state;
				}
			}
		}
		
		if (!error)
			*retval = combined_state, combined_state = NULL;
		
		arena_dealloc(throwaway_arena, iterators);
	}
	
	EXIT;
	return error;
}






















