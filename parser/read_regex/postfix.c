
#include <debug.h>

#include "high.h"
#include "postfix.h"

int read_regex_postfix(
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
	ENTER;
	
	error = read_regex_high(fd, cb, rb, cc, ct, ctd, ra,
				out_start, out_accept, out_reject);
	
	TODO;
	
	EXIT;
	return error;
}

