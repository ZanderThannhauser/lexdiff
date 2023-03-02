
#include <stdlib.h>
#include <assert.h>

#include <debug.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include <cmdln/specification_path.h>

/*#include <memory/smalloc.h>*/

#include <set/regex/new.h>
#include <set/regex/free.h>
#include <set/regex/add.h>

#include <regex/struct.h>
#include <regex/free.h>

#include <token_rule/struct.h>
#include <token_rule/new.h>
#include <token_rule/add_within.h>
/*#include <token_rule/free.h>*/

/*#include <id_to_rule/add.h>*/

#include "regex_to_nfa.h"
#include "nfas_to_dfa.h"
#include "simplify_dfa.h"
#include "parse_expression.h"
#include "zebu.h"
#include "parse.h"

struct regex* parse_specification()
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
		
		struct token_rule* rule = new_token_rule(i);
		
		regexset_add(nfas, nfa.start), nfa.accepts->accepts = rule;
		
		if (token_rule->insert)
		{
			mpq_ptr insert = parse_expression(token_rule->insert);
			mpq_set(rule->insert, insert);
			mpq_clear(insert), free(insert);
		}
		
		if (token_rule->match)
		{
			mpq_ptr match = parse_expression(token_rule->match);
			mpq_set(rule->match, match);
			mpq_clear(match), free(match);
		}
		
		if (token_rule->update)
		{
			mpq_ptr update = parse_expression(token_rule->update);
			mpq_set(rule->update, update);
			mpq_clear(update), free(update);
		}
		
		if (token_rule->delete)
		{
			mpq_ptr delete = parse_expression(token_rule->delete);
			mpq_set(rule->delete, delete);
			mpq_clear(delete), free(delete);
		}
		
		for (unsigned i = 0, n = token_rule->withins.n; i < n; i++)
		{
			struct zebu_within* within = token_rule->withins.data[i];
			
			mpq_ptr tolerance = parse_expression_highest(within->tolerance);
			
			mpq_ptr points = parse_expression(within->points);
			
			token_rule_add_within(rule, tolerance, !!within->percent, points);
			
			mpq_clear(tolerance), free(tolerance);
			mpq_clear(points), free(points);
		}
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




















