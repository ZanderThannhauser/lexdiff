
#include <debug.h>

#include <lex/nfa_node/add_lambda_branch.h>

#include "../token.h"

#include "postfix.h"
#include "juxtaposition.h"

int read_regex_juxtaposition(
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
	struct nfa_node* start;
	struct nfa_node* accept;
	ENTER;
	
	error = read_regex_postfix(fd, cb, rb, cc, ct, ctd, ra,
		&start, &accept);
	
	while (!error && !(false
		|| *ct == t_colon
		|| *ct == t_doublecolon
		|| *ct == t_vertical
		|| *ct == t_close_paren))
	{
		struct nfa_node* next_start;
		struct nfa_node* next_accept;
		
		error = 0
			?: read_regex_postfix(fd, cb, rb, cc, ct, ctd, ra,
				&next_start, &next_accept)
			?: nfa_node_add_lambda_branch(accept, ra, next_start);
		
		accept = next_accept;
	}
	
	*out_start = start;
	*out_accept = accept;
	
	EXIT;
	return error;
}



















