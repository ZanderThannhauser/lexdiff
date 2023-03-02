
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <gmp.h>

#include <debug.h>

/*#include <defines/argv0.h>*/

/*#include <enums/error.h>*/

#include <memory/smalloc.h>

/*#include <id_to_rule/get.h>*/

/*#include <cmdln/verbose.h>*/

#include <token_rule/struct.h>

/*#include <edit_kind.h>*/

/*#include <mpq_print.h>*/

#include <mpq/init_set.h>
#include <mpq/init_set_ui.h>

#include <token/struct.h>

#include <token_list/struct.h>

#include "print_token.h"
#include "diff_cell.h"
#include "diff.h"

struct diff_cell* diff(
	struct token_list* before,
	struct token_list* after)
{
	ENTER;
	
	struct diff_cell (*costs)[before->n+1][after->n+1] = smalloc(sizeof(*costs));
	
	mpq_init_set_ui(costs[0][0][0].total, 100, 1);
	mpq_init_set_ui(costs[0][0][0].delta, 0, 1);
	
	unsigned i, j, n, m;
	
	mpq_t total, delta;
	mpq_init(total), mpq_init(delta);
	mpq_set_ui(total, 100, 1);
	
	for (i = 0, n = before->n; i < n; i++)
	{
		struct diff_cell* cell = &costs[0][i+1][0];
		
		mpq_ptr delete = before->data[i]->rule->delete;
		
		mpq_add(total, total, delete);
		
		cell->action = ek_delete;
		mpq_init_set(cell->total, total);
		mpq_init_set(cell->delta, delete);
	}
	
	mpq_set_ui(total, 100, 1);
	
	for (j = 0, m = after->n; j < m; j++)
	{
		struct diff_cell* cell = &costs[0][0][j+1];
		
		mpq_ptr insert = after->data[j]->rule->insert;
		
		mpq_add(total, total, insert);
		
		cell->action = ek_insert;
		mpq_init_set(cell->total, total);
		mpq_init_set(cell->delta, insert);
	}
	
	mpq_t alt_total, alt_delta, hundred, sub, tmp;
	mpq_init(alt_total), mpq_init(alt_delta);
	mpq_init_set_ui(hundred, 100, 1);
	mpq_init(sub), mpq_init(tmp);
	enum edit_kind action;
	
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			struct token* btok = before->data[i];
			struct token* atok = after->data[j];
			struct token_rule* brule = btok->rule;
			struct token_rule* arule = atok->rule;
			
			struct diff_cell* cell = &costs[0][i+1][j+1];
			
			// consider delete:
			{
				mpq_set(delta, brule->delete);
				mpq_add(total, delta, costs[0][i][j+1].total);
				action = ek_delete;
			}
			
			// consider insert:
			{
				mpq_set(alt_delta, arule->insert);
				mpq_add(alt_total, alt_delta, costs[0][i+1][j].total);
			
				if (mpq_cmp(alt_total, total) >= 0)
				{
					mpq_set(total, alt_total);
					mpq_set(delta, alt_delta);
					action = ek_insert;
				}
			}
			
			// if the tokens are of a different kind, then they aren't 'similar'
			if (brule == arule)
			{
				if (strcmp(btok->chars, atok->chars))
				{
					mpq_add(alt_total, brule->update, costs[0][i][j].total);
					
					if (mpq_cmp(alt_total, total) >= 0)
					{
						mpq_set(delta, brule->update);
						mpq_set(total, alt_total);
						action = ek_update;
					}
				}
				else
				{
					mpq_add(alt_total, brule->match, costs[0][i][j].total);
					
					if (mpq_cmp(alt_total, total) >= 0)
					{
						mpq_set(delta, brule->match);
						mpq_set(total, alt_total);
						action = ek_match;
					}
				}
				
				unsigned k = 0, n = brule->withins.n;
				
				if (n)
				{
					mpq_sub(sub, btok->value, atok->value), mpq_abs(sub, sub);
					
					for (; k < n; k++)
					{
						struct within* ele = &brule->withins.data[k];
						
						if (ele->is_percentage)
						{
							mpq_div(tmp, sub, btok->value);
							mpq_mul(tmp, tmp, hundred);
						}
						else
						{
							mpq_set(tmp, sub);
						}
						
						if (mpq_cmp(tmp, ele->tolerance) <= 0)
						{
							mpq_add(alt_total, ele->points, costs[0][i][j].total);
							
							if (mpq_cmp(alt_total, total) > 0)
							{
								mpq_set(delta, ele->points);
								mpq_set(total, alt_total);
								action = ek_within;
								cell->within_index = k;
							}
						}
					}
				}
			}
			
			mpq_init(cell->total), mpq_set(cell->total, total);
			mpq_init(cell->delta), mpq_set(cell->delta, delta);
			cell->action = action;
		}
	}
	
	#ifdef DEBUGGING
	for (i = 0; i <= n; i++)
	{
		for (j = 0; j <= m; j++)
		{
			gmp_printf("%4Qi (%3+Qi)",
				costs[0][i][j].total,
				costs[0][i][j].delta);
			
			if (j + 1 <= m)
				printf(", ");
		}
		
		puts("");
	}
	#endif
	
	mpq_clear(alt_total), mpq_clear(alt_delta);
	
	mpq_clear(total), mpq_clear(delta);
	
	mpq_clear(hundred);
	mpq_clear(sub), mpq_clear(tmp);
	
	EXIT;
	return (void*) costs;
}

void free_diff_table(
	struct diff_cell* table,
	unsigned len)
{
	for (unsigned i = 0; i < len; i++)
	{
		mpq_clear(table[i].total);
		mpq_clear(table[i].delta);
	}
	
	free(table);
}


























