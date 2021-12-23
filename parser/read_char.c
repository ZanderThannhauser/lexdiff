
#include <debug.h>

#include <utf8/how_many.h>
#include <utf8/decode.h>

#include "read_char.h"

int read_char(
	int fd,
	uint8_t* current_byte,
	int (*read_byte)(int, uint8_t*),
	wchar_t* outgoing_char)
{
	int error = 0;
	int i, how_many;
	uint8_t buffer[6];
	ENTER;
	
	error = utf8_how_many(&how_many, *current_byte);
	
	for (i = 0; !error && i < how_many; i++)
	{
		buffer[i] = *current_byte;
		error = read_byte(fd, current_byte);
	}
	
	if (!error)
		error = utf8_decode(buffer, outgoing_char);
	
	dpvc(*outgoing_char);
	
	EXIT;
	return error;
}

