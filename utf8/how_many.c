
#include <assert.h>
// #include <stdio.h>

#include <debug.h>

#include <enums/error.h>

#include "how_many.h"

// returns how many more 'char's should be read to have all of the
// 'char's that escape this utf8 character

int utf8_how_many(int *how_many, uint8_t start_of_utf8)
{
	int error = 0;
	
	switch (start_of_utf8)
	{
		case 0b00000000 ... 0b01111111: *how_many = 1; break;
		case 0b11000000 ... 0b11011111: *how_many = 2; break;
		case 0b11100000 ... 0b11101111: *how_many = 3; break;
		case 0b11110000 ... 0b11110111: *how_many = 4; break;
		case 0b11111000 ... 0b11111011: *how_many = 5; break;
		case 0b11111100 ... 0b11111101: *how_many = 6; break;
		
		default:
		{
			error = e_malformed_unicode;
			break;
		}
	}
	
	return error;
}
