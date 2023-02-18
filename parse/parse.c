
#include <stdlib.h>
#include <assert.h>
#include <gmp.h>

#include <debug.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include <cmdln/specification_path.h>

#include <memory/smalloc.h>

#include <set/regex/new.h>
#include <set/regex/free.h>

#include <regex/struct.h>
#include <regex/free.h>

#include <set/regex/add.h>

#include <id_to_cost/set.h>

#include "regex_to_nfa.h"
#include "nfas_to_dfa.h"
#include "simplify_dfa.h"
#include "parse_expression.h"
#include "zebu.h"
#include "parse.h"

struct regex* parse_specification(
	struct id_to_cost* idtoc)
{
	ENTER;
	
	FILE* stream = fopen(specification_path, "r");
	
	if (!stream)
	{
		fprintf(stderr, "%s: fopen(\"%s\"): %m\n", argv0, specification_path);
		exit(e_syscall_failed);
	}
	
	struct regexset* nfas = new_regexset();
	
	struct zebu_$start* start = zebu_parse(stream);
	
	struct zebu_root* root = start->root;
	
	for (unsigned i = 0, n = root->rules.n; i < n; i++)
	{
		struct zebu_token_rule* token_rule = root->rules.data[i];
		
		struct nfa nfa = regex_to_nfa(token_rule->regex);
		
		regexset_add(nfas, nfa.start);
		
		unsigned id = i + 1;
		
		nfa.accepts->accepts = id;
		
		mpq_ptr insert, update, match, delete;
		
		if (token_rule->insert)
			insert = parse_expression(token_rule->insert);
		else
			insert = smalloc(sizeof(*insert)),
			mpq_init(insert), mpq_set_ui(insert, 0, 1);
		
		if (token_rule->update)
			update = parse_expression(token_rule->update);
		else
			update = smalloc(sizeof(*update)),
			mpq_init(update), mpq_set_ui(update, 0, 1);
		
		if (token_rule->match)
			match = parse_expression(token_rule->match);
		else
			match = smalloc(sizeof(*match)),
			mpq_init(match), mpq_set_ui(match, 0, 1);
		
		if (token_rule->delete)
			delete = parse_expression(token_rule->delete);
		else
			delete = smalloc(sizeof(*delete)),
			mpq_init(delete), mpq_set_ui(delete, 0, 1);
		
		id_to_cost_set(idtoc, id, insert, update, match, delete);
	}
	
	struct regex* dfa = nfas_to_dfa(nfas);
	
	struct regex* simp = simplify_dfa(dfa);
	
	free_regexset_and_elements(nfas);
	
	free_zebu_$start(start);
	
	free_regex(dfa);
	
	fclose(stream);
	
	EXIT;
	return simp;
}




















