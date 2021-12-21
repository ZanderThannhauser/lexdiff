
#include <assert.h>
#include <stdio.h>

#include <debug.h>

#include <enums/error.h>

#include "decode.h"

int utf8_decode(const uint8_t *utf8, wchar_t* codepoint)
{
	int error = 0;
	
	*codepoint = 0;
	
	switch (utf8[0])
	{
		case 0b00000000 ... 0b01111111: {
			(*codepoint) |= (0b01111111 & utf8[0]) << 0;
			break;
		}
		case 0b11000000 ... 0b11011111: {
			(*codepoint) |= (0b00011111 & utf8[0]) << 6;
			(*codepoint) |= (0b00111111 & utf8[1]) << 0;
			break;
		}
		case 0b11100000 ... 0b11101111: {
			(*codepoint) |= (0b00001111 & utf8[0]) << 12;
			(*codepoint) |= (0b00111111 & utf8[1]) << 6;
			(*codepoint) |= (0b00111111 & utf8[2]) << 0;
			break;
		}
		case 0b11110000 ... 0b11110111: {
			(*codepoint) |= (0b00000111 & utf8[0]) << 18;
			(*codepoint) |= (0b00111111 & utf8[1]) << 12;
			(*codepoint) |= (0b00111111 & utf8[2]) << 6;
			(*codepoint) |= (0b00111111 & utf8[3]) << 0;
			break;
		}
		case 0b11111000 ... 0b11111011: {
			(*codepoint) |= (0b00000011 & utf8[0]) << 24;
			(*codepoint) |= (0b00111111 & utf8[1]) << 18;
			(*codepoint) |= (0b00111111 & utf8[2]) << 12;
			(*codepoint) |= (0b00111111 & utf8[3]) << 6;
			(*codepoint) |= (0b00111111 & utf8[4]) << 0;
			break;
		}
		case 0b11111100 ... 0b11111101: {
			(*codepoint) |= (0b00000001 & utf8[0]) << 30;
			(*codepoint) |= (0b00111111 & utf8[1]) << 24;
			(*codepoint) |= (0b00111111 & utf8[2]) << 18;
			(*codepoint) |= (0b00111111 & utf8[3]) << 12;
			(*codepoint) |= (0b00111111 & utf8[4]) << 6;
			(*codepoint) |= (0b00111111 & utf8[5]) << 0;
			break;
		}
		
		default:
			error = e_malformed_unicode;
			break;
	}
	
	return error;
}



