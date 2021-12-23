
#include <debug.h>

#include <lex/nfa_node/add_lambda_branch.h>

#include "../token.h"
#include "../read_token.h"

#include "high.h"
#include "postfix.h"

int read_regex_postfix(
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
	
	error = read_regex_high(fd, cb, rb, cc, ct, ctd, ra,
		out_start, out_accept);
	
	if (!error)
	{
		switch (*ct)
		{
			case t_plus:
				error = 0
					?: nfa_node_add_lambda_branch(*out_accept, ra, *out_start)
					?: read_token(fd, cb, rb, cc, ct, ctd);
				break;
			
			case t_question:
				error = 0
					?: nfa_node_add_lambda_branch(*out_start, ra, *out_accept)
					?: read_token(fd, cb, rb, cc, ct, ctd);
				break;
			
			case t_asterisk:
				error = 0
					?: nfa_node_add_lambda_branch(*out_accept, ra, *out_start)
					?: nfa_node_add_lambda_branch(*out_start, ra, *out_accept)
					?: read_token(fd, cb, rb, cc, ct, ctd);
				break;
			
			default:
				break;
		}
	}
	
	EXIT;
	return error;
}















