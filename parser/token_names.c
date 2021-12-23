
#include "token_names.h"

const char* const token_names[number_of_tokens] = {
	[t_error] = "<error-token>",
	
	// single-character tokens:
	[t_plus] = "'+'",
	[t_minus] = "'-'",
	[t_comma] = "','",
	[t_colon] = "':'",
	[t_period] = "'.'",
	[t_carrot] = "'^'",
	[t_question] = "'?'",
	[t_vertical] = "'|'",
	[t_asterisk] = "'*'",
	[t_semicolon] = "';'",
	[t_doublecolon] = "'::'",
	
	// brackets:
	[t_open_paren] = "'('",  [t_close_paren] = "')'",
	[t_open_square] = "'['", [t_close_square] = "']'",
	
	// multicharacter tokens:
	[t_number] = "<numeric-literal>",
	[t_character] = "<character-literal>",
	[t_string] = "<string-literal>",
	
	// keywords:
	[t_options] = "<options-keyword>",
	[t_reject] = "<reject-keyword>",
	
	[t_EOF] = "EOF",
};

