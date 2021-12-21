
#include <debug.h>

#include <lex/nfa/node/new.h>
#include <lex/nfa/node/add_default_branch.h>
#include <lex/nfa/node/add_normal_branch.h>

#include "../token.h"
#include "../tokendata.h"

#include "../read_token.h"

#include "set.h"

int read_regex_set(
	int fd,
	uint8_t* cb,
	int (*rb)(int, uint8_t*),
	wchar_t* cc,
	enum token* ct,
	union tokendata* ctd,
	struct arena* ra,
	struct nfa_node** out_start,
	struct nfa_node** out_accept,
	struct nfa_node** out_reject)
{
	int error = 0;
	bool uninverted = false;
	struct nfa_node* start = NULL;
	struct nfa_node* accept = NULL;
	struct nfa_node* reject = NULL;
	ENTER;
	
	assert(*ct == t_open_square);
	
	error = read_token(fd, cb, rb, cc, ct, ctd);
	
	if (!error && *ct == t_carrot)
	{
		uninverted = true;
		error = read_token(fd, cb, rb, cc, ct, ctd);
	}
	
	if (!error)
		error = 0
			?: new_nfa_node(&start, ra)
			?: new_nfa_node(&accept, ra)
			?: new_nfa_node(&reject, ra)
			?: nfa_node_add_default_branch(start, ra, uninverted ? reject : accept);
	
	while (!error && *ct != t_close_square)
	{
		wchar_t i, startcp, endcp;
		
		if (*ct != t_character)
		{
			TODO;
			error = 1;
		}
		
		if (!error)
		{
			startcp = endcp = ctd->character.codepoint;
			error = read_token(fd, cb, rb, cc, ct, ctd);
		}
		
		if (!error && *ct == t_minus)
		{
			error = read_token(fd, cb, rb, cc, ct, ctd);
			
			if (!error && *ct != t_character)
			{
				TODO;
				error = 1;
			}
			
			if (!error)
			{
				endcp = ctd->character.codepoint;
				error = read_token(fd, cb, rb, cc, ct, ctd);
			}
		}
		
		if (!error && endcp < startcp)
		{
			TODO;
			error = 1;
		}
		
		for (i = startcp; !error && i <= endcp; i++)
		{
			dpvc(i);
			
			error = nfa_node_add_normal_branch(start, ra, i, uninverted ? accept : reject);
		}
		
		if (!error && *ct == t_comma)
			error = read_token(fd, cb, rb, cc, ct, ctd);
	}
	
	error = read_token(fd, cb, rb, cc, ct, ctd);
	
	TODO;
	
	if (!error)
	{
		*out_start = start;
		*out_accept = accept;
		*out_reject = reject;
	}
	
	EXIT;
	return error;
}



















