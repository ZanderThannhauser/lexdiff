
#include <wchar.h>
#include <inttypes.h>

enum token;
union tokendata;

int read_token(
	int fd,
	uint8_t* current_byte,
	int (*read_byte)(int, uint8_t*),
	wchar_t* current_char,
	enum token* outgoing_token,
	union tokendata* outgoing_token_data);
