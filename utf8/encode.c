
#include <assert.h>
// #include <stdio.h>
// #include <wchar.h>

#include <debug.h>

#include "encode.h"

// returns number of characters written to in 'utf8'

int utf8_encode(
	wchar_t code,	// input: codepoint to encode
	uint8_t encoding[6]) // output: up to 6 characters to encode given codepoint
{
	int n = 0;
	
	switch (code) {
		case 0x00000000 ... 0x0000007F:
			encoding[n++] = 0b00000000 | ((code >> 0) & 0b01111111);
			break;
		
		case 0x00000080 ... 0x000007FF:
			encoding[n++] = 0b11000000 | ((code >> 6) & 0b00011111);
			encoding[n++] = 0b10000000 | ((code >> 0) & 0b00111111);
			break;
		
		case 0x00000800 ... 0x0000FFFF:
			encoding[n++] = 0b11100000 | ((code >> 12) & 0b00001111);
			encoding[n++] = 0b10000000 | ((code >> 6) & 0b00111111);
			encoding[n++] = 0b10000000 | ((code >> 0) & 0b00111111);
			break;
		
		case 0x00010000 ... 0x001FFFFF:
			encoding[n++] = 0b11110000 | ((code >> 18) & 0b00000111);
			encoding[n++] = 0b10000000 | ((code >> 12) & 0b00111111);
			encoding[n++] = 0b10000000 | ((code >> 6) & 0b00111111);
			encoding[n++] = 0b10000000 | ((code >> 0) & 0b00111111);
			break;
		
		case 0x00200000 ... 0x03FFFFFF:
			encoding[n++] = 0b11111000 | ((code >> 24) & 0b00000011);
			encoding[n++] = 0b10000000 | ((code >> 18) & 0b00111111);
			encoding[n++] = 0b10000000 | ((code >> 12) & 0b00111111);
			encoding[n++] = 0b10000000 | ((code >> 6) & 0b00111111);
			encoding[n++] = 0b10000000 | ((code >> 0) & 0b00111111);
			break;
		
		case 0x04000000 ... 0x7FFFFFFF:
			encoding[n++] = 0b11111100 | ((code >> 30) & 0b00000001);
			encoding[n++] = 0b10000000 | ((code >> 24) & 0b00111111);
			encoding[n++] = 0b10000000 | ((code >> 18) & 0b00111111);
			encoding[n++] = 0b10000000 | ((code >> 12) & 0b00111111);
			encoding[n++] = 0b10000000 | ((code >> 6) & 0b00111111);
			encoding[n++] = 0b10000000 | ((code >> 0) & 0b00111111);
			break;
	}
	
	return n;
}












