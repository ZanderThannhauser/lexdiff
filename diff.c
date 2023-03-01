
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <gmp.h>

#include <debug.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include <memory/smalloc.h>

#include <id_to_rule/get.h>

#include <cmdln/verbose.h>

#include <token_rule/struct.h>

#include <edit_kind.h>

#include <mpq_print.h>

#include "token.h"
#include "print_token.h"
#include "token_list/struct.h"
#include "diff_cell.h"
#include "diff.h"

static void mpq_init_set_ui(mpq_ptr ptr, unsigned a, unsigned b)
{
	mpq_init(ptr);
	mpq_set_ui(ptr, a, b);
}

static void mpq_init_set(mpq_ptr ptr, mpq_ptr src)
{
	mpq_init(ptr);
	mpq_set(ptr, src);
}

static bool mpq_init_set_decimal(mpq_ptr ptr, const char* str)
{
	ENTER;
	
	mpq_init(ptr);
	
	mpq_t ten, inc;
	mpq_init_set_ui(ten, 10, 1);
	mpq_init_set_ui(inc, 1, 1);
	
	dpvs(str);
	
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
	
	mpq_clear(ten), mpq_clear(inc);
	
	EXIT;
	return !!*str;
}

struct diff_cell* diff(
	struct id_to_rule* idtor,
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
		
		mpq_ptr delete = id_to_rule_get_rule(idtor, before->data[i]->id)->delete;
		
		mpq_add(total, total, delete);
		
		cell->action = ek_delete;
		mpq_init_set(cell->total, total);
		mpq_init_set(cell->delta, delete);
	}
	
	mpq_set_ui(total, 100, 1);
	
	for (j = 0, m = after->n; j < m; j++)
	{
		struct diff_cell* cell = &costs[0][0][j+1];
		
		mpq_ptr insert = id_to_rule_get_rule(idtor, after->data[j]->id)->insert;
		
		mpq_add(total, total, insert);
		
		cell->action = ek_insert;
		mpq_init_set(cell->total, total);
		mpq_init_set(cell->delta, insert);
	}
	
	mpq_t alt_total, alt_delta;
	mpq_init(alt_total), mpq_init(alt_delta);
	enum edit_kind action;
	
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			unsigned mid = before->data[i]->id, cid = after->data[j]->id;
			
			struct diff_cell* cell = &costs[0][i+1][j+1];
			
			// consider delete:
			{
				mpq_set(delta, id_to_rule_get_rule(idtor, mid)->delete);
				mpq_add(total, delta, costs[0][i][j+1].total);
				action = ek_delete;
			}
			
			// consider insert:
			{
				mpq_set(alt_delta, id_to_rule_get_rule(idtor, cid)->insert);
				mpq_add(alt_total, alt_delta, costs[0][i+1][j].total);
			
				if (mpq_cmp(alt_total, total) >= 0)
				{
					mpq_set(total, alt_total);
					mpq_set(delta, alt_delta);
					action = ek_insert;
				}
			}
			
			// if the tokens are of a different kind, then they aren't 'similar'
			if (mid == cid)
			{
				struct token_rule* rule = id_to_rule_get_rule(idtor, cid);
				
				if (strcmp(before->data[i]->data, after->data[j]->data))
				{
					mpq_add(alt_total, rule->update, costs[0][i][j].total);
					
					if (mpq_cmp(alt_total, total) >= 0)
					{
						mpq_set(delta, rule->update);
						mpq_set(total, alt_total);
						action = ek_update;
					}
				}
				else
				{
					mpq_add(alt_total, rule->match, costs[0][i][j].total);
					
					if (mpq_cmp(alt_total, total) >= 0)
					{
						mpq_set(delta, rule->match);
						mpq_set(total, alt_total);
						action = ek_match;
					}
				}
				
				unsigned k = 0, n = rule->withins.n;
				
				if (n)
				{
					mpq_t mq, cq;
					
					if (mpq_init_set_decimal(mq, before->data[i]->data))
					{
						fprintf(stderr, "%s: could not convert '", argv0);
						print_token(stderr, before->data[i]->data, -1);
						fprintf(stderr, "' into a number! (from first file)\n");
						exit(e_bad_spec_file);
					}
					
					if (mpq_init_set_decimal(cq, after->data[i]->data))
					{
						fprintf(stderr, "%s: could not convert '", argv0);
						print_token(stderr, after->data[i]->data, -1);
						fprintf(stderr, "' into a number! (from second file)\n");
						exit(e_bad_spec_file);
					}
					
					mpq_t sub, tmp, hundred;
					mpq_init(sub), mpq_sub(sub, mq, cq), mpq_abs(sub, sub);
					mpq_init_set_ui(hundred, 100, 1);
					mpq_init(tmp);
					
					for (; k < n; k++)
					{
						struct within* ele = &rule->withins.data[k];
						
						if (ele->is_percentage)
						{
							mpq_div(tmp, sub, mq);
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
					
					mpq_clear(sub), mpq_clear(tmp), mpq_clear(hundred);
					mpq_clear(mq), mpq_clear(cq);
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


























