
#include <avl/avl.h>

#include <debug.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include "statelist/new.h"
#include "statelist/insert.h"

#include "push_lambda_transitions.h"
#include "compare_bundles.h"
#include "combine_states.h"
#include "free_bundle.h"

#include "convert.h"

int convert_nfa_to_dfa(
	struct nfa_node* in,
	struct memory_arena* throwaway_arena,
	struct memory_arena* dfa_arena,
	struct dfa_node** retval)
{
	int error = 0;
	avl_tree_t* tree = NULL;
	struct statelist* starting_statelist = NULL;
	ENTER;
	
	// tree maps list of dfa states to nfa states
	if (!(tree = avl_alloc_tree(compare_bundles, NULL)))
	{
		fprintf(stderr, "%s: malloc(): %m\n", argv0);
		error = e_out_of_memory;
	}
	
	// setup initial state-list:
	if (!error)
		error = 0
			?: new_statelist(&starting_statelist, throwaway_arena)
			?: statelist_insert(starting_statelist, throwaway_arena, in)
			?: push_lambda_transitions(starting_statelist, throwaway_arena)
			?: combine_states(tree, starting_statelist, throwaway_arena, dfa_arena, retval);
	
	if (tree)
		avl_free_tree(tree);
	
	EXIT;
	return error;
}

















