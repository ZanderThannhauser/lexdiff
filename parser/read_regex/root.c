
#include <debug.h>

#include "../token.h"
#include "../tokendata.h"
#include "../read_token.h"

#include "or.h"
#include "root.h"

// regex: regex-bnry;
// regex-bnry: regex-post ('|' regex-bnry)*;
// regex-juxt: regex-post regex-post*;
// regex-post: regex-high ('*' | '+' | '?')?;
// regex-high: '(' regex ')' | <string> | <char> | '[' regex-setb ']' | '.';
// regex-setb: '^'? regex-setc (',' regex-setc)*;
// regex-setc: <char> | <char> '-' <char>;

int read_regex_root(
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
	
	error = read_regex_or(fd, cb, rb, cc, ct, ctd, ra,
				out_start, out_accept, out_reject);
	
	TODO;
	
	EXIT;
	return error;
}




	#if 0
	int low()
	{
		int error = 0;
		ENTER;
		
		int binary()
		{
			int error = 0;
			ENTER;
			
			int juxtaposition()
			{
				int error = 0;
				ENTER;
				
				int postfix()
				{
					int error = 0;
					ENTER;
					
					int high()
					{
						int error = 0;
						ENTER;
						
						int set_binary()
						{
							int error = 0;
							ENTER;
							
							int set_char()
							{
								int error = 0;
								wchar_t i, start, end;
								ENTER;
								
								if (*ct != t_character)
								{
									TODO;
									error = 1;
								}
								
								if (!error)
								{
									start = ctd->character.codepoint;
									end = start;
									
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
										end = ctd->character.codepoint;
									}
								}
								
								if (!error && end < start)
								{
									TODO;
									error = 1;
								}
								
								if (!error)
								{
									dpv(start);
									dpv(end);
									
									TODO;
								}
								
								EXIT;
								return error;
							}
							
							bool negate = false;
							
							if (*ct == t_carrot)
							{
								negate = true;
								error = read_token(fd, cb, rb, cc, ct, ctd);
							}
							
							while (!error && *ct != t_close_square)
							{
								error = set_char();
							}
							
							TODO;
							
							EXIT;
							return error;
						}
						
						switch (*ct)
						{
							case t_open_paren:
								TODO;
								break;
							
							case t_open_square:
							{
								error = 0
									?: read_token(fd, cb, rb, cc, ct, ctd)
									?: set_binary()
									?: read_token(fd, cb, rb, cc, ct, ctd);
								
								TODO;
								break;
							}
							
							default:
								TODO;
								break;
						}
						
						TODO;
						
						EXIT;
						return error;
					}
					
					error = high();
					
					TODO;
					EXIT;
					return error;
				}
				
				error = postfix();
				
				TODO;
				EXIT;
				return error;
			}
			
			error = juxtaposition();
			
			TODO;
			EXIT;
			return error;
		}
		
		error = binary();
		
		TODO;
		EXIT;
		return error;
	}
	#endif
	



