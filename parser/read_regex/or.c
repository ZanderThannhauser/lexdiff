
#include <debug.h>

#include <lex/nfa_node/add_lambda_branch.h>

#include "../token.h"
#include "../read_token.h"

#include "juxtaposition.h"
#include "or.h"

int read_regex_or(
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
	struct nfa_node* sub_start;
	struct nfa_node* sub_accept;
	ENTER;
	
	error = read_regex_juxtaposition(fd, cb, rb, cc, ct, ctd, ra,
		out_start, out_accept);
	
	while (!error && *ct == t_vertical)
	{
		error = 0
			?: read_token(fd, cb, rb, cc, ct, ctd)
			?: read_regex_juxtaposition(fd, cb, rb, cc, ct, ctd, ra,
				&sub_start, &sub_accept)
			?: nfa_node_add_lambda_branch(*out_start, ra, sub_start)
			?: nfa_node_add_lambda_branch(sub_accept, ra, *out_accept);
	}
	
	EXIT;
	return error;
}
















