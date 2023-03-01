
#include <debug.h>

#include "print_token.h"

void print_token(
	FILE* stream, char* moving, unsigned len)
{
	char c;
	while (len-- && (c = *moving++))
	{
		switch (c)
		{
			case '\n': fputs("↲", stream); break;
			case '\t': fputs("↦", stream); break;
			case  ' ': fputs("␣", stream); break;
			default  : fputc( c,  stream); break;
		}
	}
}

