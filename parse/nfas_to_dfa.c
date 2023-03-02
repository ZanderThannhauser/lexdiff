
#include <stdlib.h>

#include <debug.h>

#include <memory/smalloc.h>

#include <avl/alloc_tree.h>
#include <avl/insert.h>
#include <avl/search.h>
#include <avl/free_tree.h>

#include <set/regex/inc.h>
#include <set/regex/new.h>
#include <set/regex/add.h>
#include <set/regex/compare.h>
#include <set/regex/free.h>
#include <set/regex/foreach.h>
#include <set/regex/is_nonempty.h>

#include <token_rule/struct.h>
#include <token_rule/inc.h>

#include <quack/new.h>
#include <quack/append.h>
#include <quack/is_nonempty.h>
#include <quack/pop.h>
#include <quack/free.h>

#include <regex/new.h>
#include <regex/struct.h>

#include "nfas_to_dfa.h"

struct mapping
{
	struct regexset* stateset;
	
	struct regex* combined_state;
};

static struct mapping* new_mapping(
	struct regexset* stateset,
	struct regex* state)
{
	ENTER;
	
	struct mapping* this = smalloc(sizeof(*this));
	
	this->stateset = inc_regexset(stateset);
	
	this->combined_state = state;
	
	EXIT;
	return this;
}

static int compare_mappings(const void* a, const void* b)
{
	const struct mapping *A = a, *B = b;
	
	return compare_regexsets(A->stateset, B->stateset);
}

static void free_mapping(void* a)
{
	struct mapping* this = a;
	ENTER;
	
	free_regexset(this->stateset);
	
	free(this);
	
	EXIT;
}

static void add_lambda_states(struct regexset* set, struct regex* ele)
{
	ENTER;
	
	if (regexset_add(set, ele))
		for (unsigned i = 0, n = ele->lambdas.n; i < n; i++)
			add_lambda_states(set, ele->lambdas.data[i]);
	
	EXIT;
}

struct regex* nfas_to_dfa(
	struct regexset* nfas)
{
	ENTER;
	
	struct quack* todo = new_quack();
	
	struct regex* new_start = new_regex();
	
	struct avl_tree_t* mappings = avl_alloc_tree(compare_mappings, free_mapping);
	
	{
		struct regexset* start_set = new_regexset();
		
		regexset_foreach(nfas, ({
			void callback(struct regex* regex) {
				add_lambda_states(start_set, regex);
			}
			callback;
		}));
		
		struct mapping* mapping = new_mapping(start_set, new_start);
		
		quack_append(todo, mapping);
		
		avl_insert(mappings, mapping);
		
		free_regexset(start_set);
	}
	
	while (quack_is_nonempty(todo))
	{
		struct mapping* const mapping = quack_pop(todo);
		
		struct regexset* const stateset = mapping->stateset;
		
		struct regex* const state = mapping->combined_state;
		
		{
			struct token_rule* accepts = NULL;
			
			regexset_foreach(stateset, ({
				void runme(struct regex* regex) {
					if (regex->accepts && (!accepts || regex->accepts->rank < accepts->rank))
						accepts = regex->accepts;
				}
				runme;
			}));
			
			state->accepts = inc_token_rule(accepts);
		}
		
		for (unsigned i = 0, n = 256; i < n; i++)
		{
			struct regexset* subregexset = new_regexset();
			
			regexset_foreach(stateset, ({
				void runme(struct regex* ele) {
					struct regex* to = ele->transitions[i];
					if (to) {
						add_lambda_states(subregexset, to);
					}
				}
				runme;
			}));
			
			if (regexset_is_nonempty(subregexset))
			{
				struct avl_node_t* node = avl_search(mappings, &subregexset);
				
				if (node)
				{
					struct mapping* old = node->item;
					
					state->transitions[i] = old->combined_state;
				}
				else
				{
					struct regex* substate = new_regex();
					
					struct mapping* new = new_mapping(subregexset, substate);
					
					state->transitions[i] = substate;
					
					quack_append(todo, new);
					
					avl_insert(mappings, new);
				}
			}
			
			free_regexset(subregexset);
		}
	}
	
	avl_free_tree(mappings);
	
	free_quack(todo);
	
	EXIT;
	return new_start;
}





























