
#include <math.h>

#include <debug.h>

#include <enums/error.h>

#include <defines/argv0.h>

#include <memory/arena/malloc.h>

#include <structs/user_rule.h>

#include <lex/nfa_node/struct.h>

#include "read_regex/root.h"

#include "token.h"
#include "token_names.h"
#include "token_data.h"
#include "read_token.h"
#include "read_rule.h"

int read_rule(
	int fd,
	uint8_t* cb,
	int (*rb)(int, uint8_t*),
	wchar_t* cc,
	enum token* ct,
	union tokendata* ctd,
	struct memory_arena* ra,
	unsigned rule_id,
	struct nfa_node** out_start)
{
	int error = 0;
	struct nfa_node* start;
	struct nfa_node* accept;
	ENTER;
	
	// statement: regular expression ':' weight ';'
	
	error = read_regex_root(fd, cb, rb, cc, ct, ctd, ra,
		&start, &accept);
	
	bool match_content;
	
	if (!error)
		switch (*ct)
		{
			case t_colon:
				match_content = true;
				break;
			
			case t_doublecolon:
				match_content = true;
				break;
			
			default:
			{
				dpv(*ct);
				dpvs(token_names[*ct]);
				assert(token_names[*ct]);
				
				fprintf(stderr, "%s: syntax error: "
					"unexpected %s, expecting %s or %s!\n",
					argv0, token_names[*ct],
					token_names[t_colon],
					token_names[t_doublecolon]);
				
				error = e_syntax_error;
				break;
			}
		}
	
	if (!error)
		error = read_token(fd, cb, rb, cc, ct, ctd);
	
	bool is_deduction;
	double weight;
	bool is_percentage = false;
	
	if (!error)
	{
		if (*ct == t_reject)
		{
			is_deduction = true;
			weight = -INFINITY;
			is_percentage = false;
			error = read_token(fd, cb, rb, cc, ct, ctd);
		}
		else
		{
			switch (*ct)
			{
				case t_plus:
					is_deduction = false;
					error = read_token(fd, cb, rb, cc, ct, ctd);
					break;
				case t_minus:
					is_deduction = true;
					error = read_token(fd, cb, rb, cc, ct, ctd);
					break;
				default:
					is_deduction = true;
					break;
			}
			
			if (!error && *ct != t_number)
			{
				dpv(*ct);
				
				assert(token_names[*ct]);
				
				fprintf(stderr, "%s: syntax error: "
					"unexpected %s, expecting %s!\n",
					argv0, token_names[*ct], token_names[t_number]);
				
				error = e_syntax_error;
			}
			
			if (!error)
			{
				weight = ctd->numeric;
				error = read_token(fd, cb, rb, cc, ct, ctd);
			}
			
			if (!error && *ct == t_percent)
			{
				is_percentage = true;
				error = read_token(fd, cb, rb, cc, ct, ctd);
			}
		}
	}
	
	if (!error && *ct != t_semicolon)
	{
		TODO;
		error = 1;
	}
	
	if (!error)
		error = read_token(fd, cb, rb, cc, ct, ctd);
	
	struct user_rule* rule;
	
	if (!error)
		error = arena_malloc(ra, (void**) &rule, sizeof(*rule));
	
	if (!error)
	{
		rule->id = rule_id;
		rule->match_content = match_content;
		rule->is_deduction = is_deduction;
		rule->weight = weight;
		rule->is_percentage = is_percentage;
		
		*out_start = start;
		
		accept->rule = rule;
	}
	
	EXIT;
	return error;
}

















