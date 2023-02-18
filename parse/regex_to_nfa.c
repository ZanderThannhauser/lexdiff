
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>

#include <debug.h>

#include <enums/error.h>

#include <defines/argv0.h>

#include <regex/struct.h>
#include <regex/add_lambda_transition.h>
#include <regex/new.h>

#include "zebu.h"
#include "regex_to_nfa.h"

typedef uint16_t charset_t __attribute__ ((vector_size (256)));

static unsigned read_escape(
	unsigned char** moving,
	unsigned char*  end)
{
	unsigned retval;
	ENTER;
	
	assert(*moving < end);
	
	if (**moving != '\\')
		retval = *(*moving++);
	else if (++*moving == end)
		fprintf(stderr, "%s: incomplete escape sequence!\n", argv0),
		exit(e_bad_input_file);
	else switch (**moving) {
		case 't' : retval = '\t', *moving++; break;
		case 'n' : retval = '\n', *moving++; break;
		case '\"': retval = '\"', *moving++; break;
		default:
			fprintf(stderr, "%s: unknown escape sequence '\\%c'!\n", argv0, **moving),
			exit(e_bad_input_file);
	}
	
	EXIT;
	return retval;
}

static unsigned char max(charset_t value)
{
	for (unsigned i = 255; 1 <= i + 1; i--)
		if (value[i >> 4] & (1 << (i & 0xF)))
			return i;
	return 0;
}

static unsigned char min(charset_t value)
{
	for (unsigned i = 0, n = 256; i < n; i++)
		if (value[i >> 4] & (1 << (i & 0xF)))
			return i;
	return 255;
}

static charset_t charset_to_nfa(struct zebu_charset* charset)
{
	ENTER;
	
	charset_t charset_union_to_nfa(struct zebu_charset_union* charset)
	{
		ENTER;
		
		charset_t charset_inter_to_nfa(struct zebu_charset_inter* charset)
		{
			ENTER;
			
			charset_t charset_range_to_nfa(struct zebu_charset_range* charset)
			{
				ENTER;
				
				charset_t charset_prefix_to_nfa(struct zebu_charset_prefix* charset)
				{
					ENTER;
					
					charset_t charset_high_to_nfa(struct zebu_charset_high* charset)
					{
						charset_t retval;
						ENTER;
						
						if (charset->character)
						{
							unsigned char* moving = charset->character->data + 1;
							unsigned char* end = moving + charset->character->len - 2;
							
							unsigned codepoint = read_escape(&moving, end);
							
							retval = (charset_t) {};
							
							retval[codepoint >> 4] |= 1 << (codepoint & 0xF);
						}
						else if (charset->inner)
						{
							retval = charset_to_nfa(charset->inner);
						}
						else
						{
							TODO;
						}
						
						EXIT;
						return retval;
					}
					
					charset_t retval = charset_high_to_nfa(charset->inner);
					
					if (charset->emark)
						retval = ~retval;
					
					EXIT;
					return retval;
				}
				
				
				charset_t left = charset_prefix_to_nfa(charset->left);
				
				if (charset->right)
				{
					charset_t right = charset_prefix_to_nfa(charset->right);
					
					unsigned char l = min(left), r = max(right);
					
					for (unsigned i = l; i <= r; i++)
						left[i >> 4] |= (1 << (i & 0xF));
				}
				
				EXIT;
				return left;
			}
			
			charset_t inner = charset_range_to_nfa(charset->inner);
			
			if (charset->sub)
				inner &= charset_inter_to_nfa(charset->sub);
			
			EXIT;
			return inner;
		}
		
		charset_t inner = charset_inter_to_nfa(charset->inner);
		
		if (charset->sub)
			inner |= charset_union_to_nfa(charset->sub);
		
		EXIT;
		return inner;
	}
	
	charset_t inner = charset_union_to_nfa(charset->inner);
	
	EXIT;
	return inner;
}

struct nfa regex_to_nfa(struct zebu_regex* regex)
{
	struct nfa retval;
	ENTER;
	
	struct nfa regex_union_to_nfa(struct zebu_regex_union* regex)
	{
		struct nfa retval;
		ENTER;
		
		struct nfa regex_concat_to_nfa(struct zebu_regex_concat* regex)
		{
			struct nfa retval;
			ENTER;
			
			struct nfa regex_suffix_to_nfa(struct zebu_regex_suffix* regex)
			{
				struct nfa retval;
				ENTER;
				
				struct nfa regex_high_to_nfa(struct zebu_regex_high* regex)
				{
					struct nfa retval;
					ENTER;
					
					if (regex->string)
					{
						retval.start = retval.accepts = new_regex();
						
						unsigned char* moving = regex->string->data + 1;
						unsigned char* end = moving + regex->string->len - 2;
						
						while (moving < end)
						{
							unsigned codepoint = read_escape(&moving, end);
							
							struct regex* temp = new_regex();
							
							retval.accepts->transitions[codepoint] = temp;
							
							retval.accepts = temp;
						}
					}
					else if (regex->character)
					{
						unsigned char* moving = regex->character->data + 1;
						unsigned char* end = moving + regex->character->len - 2;
						
						unsigned codepoint = read_escape(&moving, end);
						
						retval.start = new_regex();
						retval.accepts = new_regex();
						
						retval.start->transitions[codepoint] = retval.accepts;
					}
					else if (regex->charset)
					{
						charset_t set = charset_to_nfa(regex->charset);
						
						retval.start = new_regex();
						retval.accepts = new_regex();
						
						for (unsigned i = 0, n = 256; i < n; i++)
							if (set[i >> 4] & 1 << (i & 0xF))
								retval.start->transitions[i] = retval.accepts;
					}
					else if (regex->sub)
					{
						retval = regex_to_nfa(regex->sub);
					}
					else
					{
						TODO;
					}
					
					EXIT;
					return retval;
				}
				
				retval = regex_high_to_nfa(regex->inner);
				
				if (regex->qmark)
				{
					regex_add_lambda_transition(retval.start, retval.accepts);
				}
				else if (regex->asterisk)
				{
					struct regex* new_accepts = new_regex();
					
					regex_add_lambda_transition(retval.start, retval.accepts);
					
					regex_add_lambda_transition(retval.accepts, retval.start);
					
					regex_add_lambda_transition(retval.accepts, new_accepts);
					
					retval.accepts = new_accepts;
				}
				else if (regex->plus)
				{
					struct regex* new_accepts = new_regex();
					
					regex_add_lambda_transition(retval.accepts, retval.start);
					
					regex_add_lambda_transition(retval.accepts, new_accepts);
					
					retval.accepts = new_accepts;
				}
				
				EXIT;
				return retval;
			}
			
			retval = regex_suffix_to_nfa(regex->inner);
			
			if (regex->sub)
			{
				struct nfa sub = regex_concat_to_nfa(regex->sub);
				
				regex_add_lambda_transition(retval.accepts, sub.start);
				
				retval.accepts = sub.accepts;
			}
			
			EXIT;
			return retval;
		}
		
		retval = regex_concat_to_nfa(regex->inner);
		
		if (regex->sub)
		{
			struct nfa sub = regex_union_to_nfa(regex->sub);
			
			regex_add_lambda_transition(retval.start, sub.start);
			
			regex_add_lambda_transition(sub.accepts, retval.accepts);
		}
		
		EXIT;
		return retval;
	}
	
	retval = regex_union_to_nfa(regex->inner);
	
	EXIT;
	return retval;
}












