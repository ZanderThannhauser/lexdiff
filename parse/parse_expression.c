
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <debug.h>

#include <memory/smalloc.h>

#include "zebu.h"
#include "parse_expression.h"

mpq_ptr parse_expression_highest(
	struct zebu_expression_highest* expression)
{
	mpq_ptr retval;
	ENTER;
	
	if (false
		|| expression->decimal
		|| expression->binary
		|| expression->octal
		|| expression->hexadecimal)
	{
		mpq_t base;
		unsigned char* start;
		
		if (expression->decimal)
		{
			mpq_init(base), mpq_set_ui(base, 10, 1);
			start = expression->decimal->data;
		}
		else if (expression->binary)
		{
			mpq_init(base), mpq_set_ui(base, 2, 1);
			start = expression->binary->data + 2;
		}
		else if (expression->octal)
		{
			mpq_init(base), mpq_set_ui(base, 8, 1);
			start = expression->octal->data + 2;
		}
		else if (expression->hexadecimal)
		{
			mpq_init(base), mpq_set_ui(base, 16, 1);
			start = expression->hexadecimal->data + 2;
		}
		else
		{
			TODO;
		}
		
		mpq_t inc;
		mpq_init(inc), mpq_set_ui(inc, 1, 1);
		mpq_init(retval = smalloc(sizeof(*retval)));
		
		while (*start && *start != '.')
		{
			mpq_mul(retval, retval, base);
			
			while ((*start)-- > '0')
				mpq_add(retval, retval, inc);
			
			start++;
		}
		
		if (*start == '.')
		{
			start++;
			
			while (*start)
			{
				mpq_div(inc, inc, base);
				
				while ((*start)-- > '0')
					mpq_add(retval, retval, inc);
				
				start++;
			}
		}
		
		mpq_clear(inc), mpq_clear(base);
	}
	else if (expression->inner)
	{
		retval = parse_expression(expression->inner);
	}
	else
	{
		TODO;
	}
	
	EXIT;
	return retval;
}
				
			
mpq_ptr parse_expression_multiplicative(
	struct zebu_expression_multiplicative* expression)
{
	mpq_ptr retval;
	ENTER;
	
	mpq_ptr parse_expression_prefix(
		struct zebu_expression_prefix* expression)
	{
		mpq_ptr retval;
		ENTER;
		
		retval = parse_expression_highest(expression->inner);
		
		if (expression->negate)
		{
			mpq_neg(retval, retval);
		}
		
		EXIT;
		return retval;
	}
	
	if (expression->inner)
	{
		retval = parse_expression_prefix(expression->inner);
	}
	else if (expression->left)
	{
		retval = parse_expression_multiplicative(expression->left);
		
		mpq_ptr right = parse_expression_prefix(expression->right);
		
		if (expression->mul)
			mpq_mul(retval, retval, right);
		else if (expression->div)
			mpq_div(retval, retval, right);
		else
		{
			TODO;
		}
		
		mpq_clear(right), free(right);
	}
	else
	{
		TODO;
	}
	
	EXIT;
	return retval;
}
	
mpq_ptr parse_expression_additive(
	struct zebu_expression_additive* expression)
{
	mpq_ptr retval;
	ENTER;
	
	
	if (expression->inner)
	{
		retval = parse_expression_multiplicative(expression->inner);
	}
	else if (expression->left)
	{
		retval = parse_expression_additive(expression->left);
		
		mpq_ptr right = parse_expression_multiplicative(expression->right);
		
		if (expression->add)
			mpq_add(retval, retval, right);
		else if (expression->sub)
			mpq_sub(retval, retval, right);
		else
		{
			TODO;
		}
		
		mpq_clear(right), free(right);
	}
	else
	{
		TODO;
	}
	
	EXIT;
	return retval;
}

mpq_ptr parse_expression(
	struct zebu_expression* expression)
{
	mpq_ptr retval;
	ENTER;
	
	retval = parse_expression_additive(expression->inner);
	
	EXIT;
	return retval;
}












