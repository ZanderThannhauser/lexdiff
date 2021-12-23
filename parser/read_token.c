
#include <stdlib.h>

#include <stdbool.h>

#include <debug.h>

#include <enums/error.h>

#include <defines/argv0.h>

#include <memory/srealloc.h>

#include <macros/min.h>

#include "token.h"
#include "token_data.h"
#include "read_char.h"
#include "read_token.h"

static const enum tokenizer_state {
	ts_error = t_error,
	
	ts_start = number_of_tokens,
	
	ts_comment,
	
	ts_plus,
	ts_colon,
	ts_comma,
	ts_minus,
	ts_percent,
	ts_asterisk,
	ts_vertical,
	ts_question,
	ts_semicolon,
	ts_doublecolon,
	
	// brakets:
	ts_open_square,
	ts_close_square,
	ts_open_paren,
	ts_close_paren,
	
	// keywords:
	ts_o, ts_op, ts_opt, ts_opti, ts_optio, ts_option, ts_options,
	ts_r, ts_re, ts_rej, ts_reje, ts_rejec, ts_reject,
	
	// numberic token:
	ts_number, ts_number2,
	
	// character states:
	ts_before_character, ts_character_escape, ts_after_character, ts_character,
	
	// string states:
	ts_building_string, ts_string_escape, ts_string,
	
	number_of_tokenizer_states,
} lookup[number_of_tokenizer_states][128 + 1] = {
	#define ALL 0 ... 128
	
	[ts_start][0] = t_EOF,
	
	[ts_start][' ']  = ts_start,
	[ts_start]['\t'] = ts_start,
	[ts_start]['\r'] = ts_start,
	[ts_start]['\n'] = ts_start,
	
	[ts_start]['#'] = ts_comment,
		[ts_comment][ALL] = ts_comment,
		[ts_comment]['\n'] = ts_start,
	
	// single character tokens:
	[ts_start]['+'] = ts_plus,      [ts_plus     ][ALL] = t_plus,
	[ts_start][','] = ts_comma,     [ts_comma    ][ALL] = t_comma,
	[ts_start]['-'] = ts_minus,     [ts_minus    ][ALL] = t_minus,
	[ts_start]['%'] = ts_percent,   [ts_percent  ][ALL] = t_percent,
	[ts_start]['*'] = ts_asterisk,  [ts_asterisk ][ALL] = t_asterisk,
	[ts_start]['?'] = ts_question,  [ts_question ][ALL] = t_question,
	[ts_start]['|'] = ts_vertical,  [ts_vertical ][ALL] = t_vertical,
	[ts_start][';'] = ts_semicolon, [ts_semicolon][ALL] = t_semicolon,
	
	// brakets:
	[ts_start]['['] = ts_open_square,  [ts_open_square ][ALL] = t_open_square,
	[ts_start][']'] = ts_close_square, [ts_close_square][ALL] = t_close_square,
	[ts_start]['('] = ts_open_paren,  [ts_open_paren ][ALL] = t_open_paren,
	[ts_start][')'] = ts_close_paren, [ts_close_paren][ALL] = t_close_paren,
	
	// ":" and "::":
	[ts_start][':'] = ts_colon,
		[ts_colon][ALL] = t_colon,
		[ts_colon][':'] = ts_doublecolon,
			[ts_doublecolon][ALL] = t_doublecolon,
	
	// "options" keyword:
	[ts_start]      ['o'] = ts_o,
		[ts_o      ]['p'] = ts_op,
		[ts_op     ]['t'] = ts_opt,
		[ts_opt    ]['i'] = ts_opti,
		[ts_opti   ]['o'] = ts_optio,
		[ts_optio  ]['n'] = ts_option,
		[ts_option ]['s'] = ts_options,
		[ts_options][ALL] = t_options,
	
	// "reject" keyword:
	[ts_start]     ['r'] = ts_r,
		[ts_r     ]['e'] = ts_re,
		[ts_re    ]['j'] = ts_rej,
		[ts_rej   ]['e'] = ts_reje,
		[ts_reje  ]['c'] = ts_rejec,
		[ts_rejec ]['t'] = ts_reject,
		[ts_reject][ALL] = t_reject,
	
	// number token:
	[ts_start]['0' ... '9'] = ts_number,
		[ts_number][ALL] = t_number,
		[ts_number]['0' ... '9'] = ts_number,
		[ts_number]['.'] = ts_number2,
			[ts_number2][ALL] = t_number,
			[ts_number2]['0' ... '9'] = ts_number2,
	
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
		dpvc(*cc);
		dpv(*cc);
		
		tstate = lookup[tstate][min(*cc, 128)];
		
		if (tstate == ts_start)
			n = 0;
		else if (tstate > ts_start)
		{
			if (n + 1 >= cap)
				error = srealloc((void**) &buffer, sizeof(*buffer) * (cap = cap * 2 ?: 1));
			
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
			case t_plus:
			case t_colon:
			case t_minus:
			case t_asterisk:
			case t_semicolon:
			case t_open_square:
			case t_close_square:
			case t_open_paren:
			case t_close_paren:
			case t_doublecolon:
			case t_question:
			case t_percent:
			case t_comma:
			case t_vertical:
				*ct = tstate;
				break;
			
			// keywords:
			case t_options:
			case t_reject:
				*ct = tstate;
				break;
			
			// EOF:
			case t_EOF:
				*ct = t_EOF;
				break;
			
			case t_number:
			{
				wchar_t* m;
				double number;
				
				if (n + 1 >= cap)
					error = srealloc((void**) &buffer, sizeof(buffer) * (cap = cap * 2 ?: 1));
				
				if (!error)
					buffer[n] = L'\0';
				
				if (!error && (errno = 0, number = wcstod(buffer, &m), errno || *m))
				{
					TODO;
					error = 1;
				}
				else
				{
					dpv(number);
					
					ctd->numeric = number;
					*ct = t_number;
				}
				
				break;
			}
			
			wchar_t *read, *write, *end;
			
			void again()
			{
				if (*read != '\\')
					*write++ = *read++;
				else switch (read++, *read++)
				{
					case 't': *write++ = '\t'; break;
					case 'r': *write++ = '\r'; break;
					case 'n': *write++ = '\n'; break;
					
					default:
						fprintf(stderr, "%s: unknown escape character: '\\%c'!\n",
							argv0, read[-1]);
						error = e_syntax_error;
						break;
				}
			}
			
			case t_character:
				read = buffer + 1, write = buffer;
				
				again();
				
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
					again();
				}
				
				n = write - buffer;
				dpvsn(buffer, (int) n);
				
				*ct = t_string;
				ctd->string.data = buffer, buffer = NULL;
				ctd->string.len = n;
				
				break;
			
			
			default:
			{
				if (n + 1 >= cap)
					error = srealloc((void**) &buffer, sizeof(buffer) * (cap = cap * 2 ?: 1));
				
				if (!error)
					buffer[n] = L'\0';
				
				fprintf(stderr, "%s: unrecogized token \"%ls\"!\n", argv0, buffer);
				
				error = e_syntax_error;
				break;
			}
		}
		#pragma GCC diagnostic pop
	}
	
	free(buffer);
	
	EXIT;
	return error;
}





























