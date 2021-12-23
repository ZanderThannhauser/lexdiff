
#include <stdio.h>

#include <debug.h>

#include <enums/error.h>

#include <defines/argv0.h>

#include <lex/nfa_node/new.h>
#include <lex/nfa_node/set_default_branch.h>
#include <lex/nfa_node/add_normal_branch.h>

#include "../token.h"
#include "../token_data.h"
#include "../read_token.h"
#include "../token_names.h"

#include "set.h"
#include "root.h"
#include "high.h"

int read_regex_high(
	int fd,
	uint8_t* cb,
	int (*rb)(int, uint8_t*),
	wchar_t* cc,
	enum token* ct,
	union tokendata* ctd,
	struct memory_arena* ra,
	struct nfa_node** out_start,
	struct nfa_node** out_accept)
{
	int error = 0;
	ENTER;
	
	switch (*ct)
	{
		case t_open_paren:
			error = 0
				?: read_token(fd, cb, rb, cc, ct, ctd)
				?: read_regex_root(
					fd, cb, rb, cc, ct, ctd, ra,
					out_start, out_accept)
				?: read_token(fd, cb, rb, cc, ct, ctd);
			break;
		
		case t_string:
		{
			size_t i, n;
			struct nfa_node *start, *accept;
			
			error = new_nfa_node(&start, ra);
			
			for (accept = start, i = 0, n = ctd->string.len; !error && i < n; i++)
			{
				struct nfa_node* node;
				
				error = 0
					?: new_nfa_node(&node, ra)
					?: nfa_node_add_normal_branch(accept, ra, ctd->string.data[i], node);
				
				accept = node;
			}
			
			free(ctd->string.data);
			
			if (!error)
				error = read_token(fd, cb, rb, cc, ct, ctd);
			
			*out_start = start;
			*out_accept = accept;
			
			break;
		}
		
		case t_open_square:
			error = read_regex_set(fd, cb, rb, cc, ct, ctd, ra,
				out_start, out_accept);
			break;
		
		case t_period:
			TODO;
			break;
		
		default:
		{
			dpv(*ct)
			
			assert(token_names[*ct]);
			
			fprintf(stderr, "%s: unexpected %s, "
				"expecting %s, %s, %s, or %s!\n",
				argv0,
				token_names[*ct],
				token_names[t_open_paren],
				token_names[t_string],
				token_names[t_open_square],
				token_names[t_period]);
			
			error = e_syntax_error;
			break;
		}
	}
	
	EXIT;
	return error;
}











