
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <gmp.h>

#include <debug.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include <mpq/init_set_ui.h>

#include "struct.h"
#include "parse_as_number.h"

static bool mpq_set_decimal(mpq_ptr ptr, const char* str)
{
	ENTER;
	
	mpq_t ten, inc;
	mpq_init_set_ui(ten, 10, 1);
	mpq_init_set_ui(inc, 1, 1);
	
	dpvs(str);
	
	bool is_negative = false;
	if (*str == '-')
		is_negative = true;
	else if (*str == '+')
		str++;
	
	while (*str && index("0123456789", *str))
	{
		mpq_mul(ptr, ptr, ten);
		
		for (char c = *str; c > '0'; c--)
			mpq_add(ptr, ptr, inc);
		
		str++;
	}
	
	if (*str == '.')
	{
		str++;
		
		while (*str && index("0123456789", *str))
		{
			mpq_div(inc, inc, ten);
			
			for (char c = *str; c > '0'; c--)
				mpq_add(ptr, ptr, inc);
			
			str++;
		}
	}
	
	if (is_negative)
		mpq_neg(ptr, ptr);
	
	mpq_clear(ten), mpq_clear(inc);
	
	EXIT;
	return !!*str;
}

void token_parse_as_number(
	struct token* this,
	unsigned line)
{
	ENTER;
	
	dpvs(this->chars);
	
	if (mpq_set_decimal(this->value, this->chars))
	{
		fprintf(stderr, "%s: cannot convert '%s' to a number on line %u!\n", argv0, this->chars, line);
		exit(e_bad_compare_files);
	}
	
	EXIT;
}




