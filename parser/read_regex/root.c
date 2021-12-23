
#include <debug.h>

#include "../token.h"
#include "../token_data.h"
#include "../read_token.h"

#include "or.h"
#include "root.h"

// regex-setc: <char> | <char> '-' <char>;
// regex-setb: '^'? regex-setc (',' regex-setc)*;
// regex-high: '(' regex ')' | <string> | <char> | '[' regex-setb ']' | '.';
// regex-post: regex-high ('*' | '+' | '?')?;
// regex-juxt: regex-post regex-post*;
// regex-bnry: regex-post ('|' regex-bnry)*;
// regex: regex-bnry;

int read_regex_root(
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
	
	error = read_regex_or(fd, cb, rb, cc, ct, ctd, ra,
		out_start, out_accept);
	
	EXIT;
	return error;
}







