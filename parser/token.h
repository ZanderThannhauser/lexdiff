
enum token
{
	t_error,
	
	// single-character tokens:
	t_semicolon,
	t_comma,
	t_colon,
	t_carrot,
	t_minus,
	
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

