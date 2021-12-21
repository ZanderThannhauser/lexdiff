
#include <inttypes.h>
#include <wchar.h>

int read_char(
	int fd,
	uint8_t* current_byte,
	int (*read_byte)(int, uint8_t*),
	wchar_t* outgoing_char);
