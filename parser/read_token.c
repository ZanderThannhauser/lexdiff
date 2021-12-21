
#include <debug.h>

#include <memory/srealloc.h>

#include <macros/min.h>

#include "token.h"
#include "tokendata.h"
#include "read_char.h"
#include "read_token.h"

static const enum tokenizer_state {
	ts_error = t_error,
	
	ts_start = number_of_tokens,
	
	ts_comment,
	
	ts_colon,
	ts_minus,
	ts_semicolon,
	ts_open_square,
	ts_close_square,
	
	ts_o, ts_op, ts_opt, ts_opti, ts_optio, ts_option, ts_options,
	
	// character states:
	ts_before_character, ts_character_escape, ts_after_character, ts_character,
	
	// string states:
	ts_building_string, ts_string_escape, ts_string,
	
	number_of_tokenizer_states,
} lookup[number_of_tokenizer_states][128 + 1] = {
	#define ALL 0 ... 128
	
	[ts_start]['\0'] = t_EOF,
	
	[ts_start][' ']  = ts_start,
	[ts_start]['\t'] = ts_start,
	[ts_start]['\r'] = ts_start,
	[ts_start]['\n'] = ts_start,
	
	[ts_start]['#'] = ts_comment,
		[ts_comment][ALL] = ts_comment,
		[ts_comment]['\n'] = ts_start,
	
	// single character tokens:
	[ts_start][':'] = ts_colon,        [ts_colon       ][ALL] = t_colon,
	[ts_start]['-'] = ts_minus,        [ts_minus       ][ALL] = t_minus,
	[ts_start][';'] = ts_semicolon,    [ts_semicolon   ][ALL] = t_semicolon,
	[ts_start]['['] = ts_open_square,  [ts_open_square ][ALL] = t_open_square,
	[ts_start][']'] = ts_close_square, [ts_close_square][ALL] = t_close_square,
	
	// "options" keyword:
	[ts_start]      ['o'] = ts_o,
		[ts_o      ]['p'] = ts_op,
		[ts_op     ]['t'] = ts_opt,
		[ts_opt    ]['i'] = ts_opti,
		[ts_opti   ]['o'] = ts_optio,
		[ts_optio  ]['n'] = ts_option,
		[ts_option ]['s'] = ts_options,
		[ts_options][ALL] = t_options,
	
	// number token:
	// [0-9]+          [Ee][+-]?[0-9]+
	// [0-9]*"."[0-9]+([Ee][+-]?[0-9]+)?
	// [0-9]+"."[0-9]*([Ee][+-]?[0-9]+)?
	// [ts_start]
	
	// character token:
	[ts_start]['\''] = ts_before_character,
		[ts_before_character][ALL] = ts_after_character,
		[ts_before_character]['\\'] = ts_character_escape,
			[ts_character_escape]['\\'] = ts_after_character,
			[ts_character_escape]['\"'] = ts_after_character,
			[ts_character_escape][ 'r'] = ts_after_character,
			[ts_character_escape][ 't'] = ts_after_character,
			[ts_character_escape][ 'n'] = ts_after_character,
			[ts_character_escape][ '0'] = ts_after_character,
		[ts_after_character]['\''] = ts_character,
		[ts_character][ALL] = t_character,
		
	// string token:
	[ts_start]['\"'] = ts_building_string,
		[ts_building_string][ALL] = ts_building_string,
		[ts_building_string]['\\'] = ts_string_escape,
			[ts_string_escape]['\\'] = ts_building_string,
			[ts_string_escape]['\"'] = ts_building_string,
			[ts_string_escape][ 'r'] = ts_building_string,
			[ts_string_escape][ 't'] = ts_building_string,
			[ts_string_escape][ 'n'] = ts_building_string,
			[ts_string_escape][ '0'] = ts_building_string,
		[ts_building_string]['\"'] = ts_string,
		[ts_string][ALL] = t_string,
};

int read_token(
	int fd,
	uint8_t* cb,
	int (*rb)(int, uint8_t*),
	wchar_t* cc,
	enum token* ct,
	union tokendata* ctd)
{
	int error = 0;
	bool keep_going = true;
	wchar_t* buffer = NULL;
	size_t i, n = 0, cap = 0;
	enum tokenizer_state tstate = ts_start;
	ENTER;
	
	while (!error && tstate >= ts_start)
	{
/*		dpvc(*cc);*/
		
		tstate = lookup[tstate][min(*cc, 128)];
		
		if (tstate == ts_start)
			n = 0;
		else if (tstate > ts_start)
		{
			if (n + 1 >= cap)
				error = srealloc((void**) &buffer, sizeof(buffer) * (cap = cap * 2 ?: 1));
			
			if (!error)
				buffer[n++] = *cc;
		}
		
		if (tstate >= ts_start)
		{
			error = read_char(fd, cb, rb, cc);
		}
	}
	
	dpvsn(buffer, (int) n);
	
	if (!error)
	{
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wswitch"
		switch (tstate)
		{
			// symbols:
			case t_colon:
			case t_minus:
			case t_semicolon:
			case t_open_square:
			case t_close_square:
				*ct = tstate;
				break;
			
			// keywords:
			case t_options:
				*ct = tstate;
				break;
			
			wchar_t *read, *write, *end;
			case t_character:
				read = buffer + 1, write = buffer;
				
				if (*read != '\\')
					*write++ = *read++;
				else switch (read++, *read)
				{
					default:
						dpvc(*read);
						TODO;
						break;
				}
				
				n = write - buffer;
				dpvsn(buffer, (int) n);
				
				*ct = t_character;
				ctd->character.codepoint = buffer[0];
				
				break;
			
			case t_string:
				
				for (
					read = buffer + 1,
					write = buffer,
					end = buffer + n - 1;
					read < end; )
				{
					if (*read != '\\')
						*write++ = *read++;
					else switch (read++, *read)
					{
						default:
							dpvc(*read);
							TODO;
							break;
					}
				}
				
				n = write - buffer;
				dpvsn(buffer, (int) n);
				
				*ct = t_string;
				ctd->string.data = buffer, buffer = NULL;
				ctd->string.len = n;
				
				break;
			
			default:
				dpvc(*cc);
				dpv(tstate);
				TODO;
				break;
		}
		#pragma GCC diagnostic pop
	}
	
	free(buffer);
	
	EXIT;
	return error;
}





























