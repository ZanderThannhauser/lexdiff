
enum token;
union tokendata;
struct arena;

int read_statement(
	int fd,
	uint8_t* current_byte,
	int (*read_byte)(int, uint8_t*),
	wchar_t* current_char,
	enum token* current_token,
	union tokendata* current_token_data,
	struct arena* regex_arena);
