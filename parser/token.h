
#ifndef ENUM_TOKEN_H
#define ENUM_TOKEN_H

enum token
{
	t_error,
	
	// single-character tokens:
	t_plus,
	t_minus,
	t_comma,
	t_colon,
	t_period,
	t_carrot,
	t_percent,
	t_vertical,
	t_question,
	t_asterisk,
	t_semicolon,
	t_doublecolon,
	
	// brackets:
	t_open_paren,  t_close_paren,
	t_open_square, t_close_square,
	
	// multicharacter tokens:
	t_number,
	t_character,
	t_string,
	
	// keywords:
	t_options,
	t_reject,
	
	t_EOF,
	
	number_of_tokens,
};

#endif
