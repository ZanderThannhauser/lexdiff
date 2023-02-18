
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <debug.h>

#include <memory/smalloc.h>

#include "zebu.h"
#include "parse_expression.h"

mpq_ptr parse_expression(
	struct zebu_expression* expression)
{
	mpq_ptr retval;
	ENTER;
	
	mpq_ptr parse_expression_additive(
		struct zebu_expression_additive* expression)
	{
		mpq_ptr retval;
		ENTER;
		
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
				
				mpq_ptr parse_expression_highest(
					struct zebu_expression_highest* expression)
				{
					mpq_ptr retval;
					ENTER;
					
					if (expression->literal)
					{
						char* str = (char*) expression->literal->data;
						
						mpq_init(retval = smalloc(sizeof(*retval)));
						
						char* dot = strchr(str, '.');
						
						if (dot)
						{
							*dot = 0;
							
							mpq_set_str(retval, str, 10);
							
							mpq_t decimal, ten;
							
							mpq_init(decimal);
							mpq_init(ten);
							
							mpq_set_ui(decimal, 1, 10);
							mpq_set_ui(ten, 10, 1);
							
							for (char* m = dot + 1; *m; m++)
							{
								dpvc(*m);
								
								for (char x = *m; x > '0'; x--)
									mpq_add(retval, retval, decimal);
								
								mpq_div(decimal, decimal, ten);
							}
							
							mpq_clear(decimal);
							mpq_clear(ten);
						}
						else
						{
							mpq_set_str(retval, str, 10);
						}
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
	
	retval = parse_expression_additive(expression->inner);
	
	EXIT;
	return retval;
}












