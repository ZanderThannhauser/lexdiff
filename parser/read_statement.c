
#include <debug.h>

#include "read_regex/root.h"

#include "read_statement.h"

int read_statement(
	int fd,
	uint8_t* cb,
	int (*rb)(int, uint8_t*),
	wchar_t* cc,
	enum token* ct,
	union tokendata* ctd,
	struct arena* regex_arena)
{
	int error = 0;
	struct nfa_node* start;
	struct nfa_node* accept;
	struct nfa_node* reject;
	ENTER;
	
	// every statement is a regular expression ':' weight ';'
	
	error = read_regex_root(fd, cb, rb, cc, ct, ctd, regex_arena, &start, &accept, &reject);
	
	TODO;
	
	EXIT;
	return error;
}

