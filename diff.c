
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <gmp.h>

#include <memory/smalloc.h>

#include <debug.h>

#include <id_to_rule/get.h>

#include <cmdln/verbose.h>

#include <token_rule/struct.h>

#include <edit_kind.h>

#include <mpq_print.h>

#include "token.h"
#include "token_list/struct.h"
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

static void mpq_init_set_decimal(mpq_ptr ptr, const char* str)
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
		
		TODO;
		
		while (*str && index("0123456789", *str))
		{
			TODO;
		}
	}
	
	if (*str)
	{
		TODO;
	}
	
	mpq_clear(ten), mpq_clear(inc);
	
	EXIT;
}

unsigned diff(
	struct id_to_rule* idtor,
	enum edit_kind* edits,
	struct token_list* before,
	struct token_list* after)
{
	ENTER;
	
	struct cell {
		enum edit_kind action;
		mpq_t score;
	} (*costs)[before->n+1][after->n+1] = smalloc(sizeof(*costs));
	
	mpq_init_set_ui(costs[0][0][0].score, 100, 1);
	
	unsigned i, j, n, m;
	
	mpq_t q;
	mpq_init_set_ui(q, 0, 1);
	
	for (i = 0, n = before->n; i < n; i++)
	{
		struct cell* cell = &costs[0][i+1][0];
		
		mpq_add(q, q, id_to_rule_get_rule(idtor, before->data[i]->id)->delete);
		
		cell->action = ek_delete;
		mpq_init_set(cell->score, q);
	}
	
	mpq_set_ui(q, 0, 1);
	
	for (j = 0, m = after->n; j < m; j++)
	{
		struct cell* cell = &costs[0][0][j+1];
		
		mpq_add(q, q, id_to_rule_get_rule(idtor, after->data[j]->id)->insert);
		
		cell->action = ek_insert;
		mpq_init_set(cell->score, q);
	}
	
	mpq_t a, b;
	mpq_init(a), mpq_init(b);
	
	enum edit_kind action;
	
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			unsigned mid = before->data[i]->id, cid = after->data[j]->id;
			
			struct cell* cell = &costs[0][i+1][j+1];
			
			// consider delete:
			{
				mpq_add(a, costs[0][i][j+1].score, id_to_rule_get_rule(idtor, mid)->delete);
				action = ek_delete;
			}
			
			// consider insert:
			{
				mpq_add(b, costs[0][i+1][j].score, id_to_rule_get_rule(idtor, cid)->insert);
			
				if (mpq_cmp(a, b) <= 0)
				{
					mpq_set(a, b);
					action = ek_insert;
				}
			}
			
			// if the tokens are of a different kind, then they aren't 'similar'
			if (mid == cid)
			{
				struct token_rule* rule = id_to_rule_get_rule(idtor, cid);
				
				if (strcmp(before->data[i]->data, after->data[j]->data))
				{
					mpq_add(b, rule->update, costs[0][i][j].score);
					
					if (mpq_cmp(a, b) <= 0)
					{
						mpq_set(a, b);
						action = ek_update;
					}
				}
				else
				{
					mpq_add(b, rule->match, costs[0][i][j].score);
					
					if (mpq_cmp(a, b) <= 0)
					{
						mpq_set(a, b);
						action = ek_match;
					}
				}
				
				unsigned k = 0, n = rule->withins.n;
				
				if (n)
				{
					mpq_t mq, cq;
					
					mpq_init_set_decimal(mq, before->data[i]->data);
					mpq_init_set_decimal(cq, after->data[i]->data);
					
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
							mpq_add(b, ele->points, costs[0][i][j].score);
							
							if (mpq_cmp(a, b) < 0)
							{
								mpq_set(a, b);
								action = ek_within;
							}
						}
					}
					
					mpq_clear(sub), mpq_clear(tmp), mpq_clear(hundred);
					mpq_clear(mq), mpq_clear(cq);
				}
			}
			
			mpq_init(cell->score), mpq_set(cell->score, a);
			cell->action = action;
		}
	}
	
	#ifdef DEBUGGING
	for (i = 0; i <= n; i++)
	{
		for (j = 0; j <= m; j++)
		{
			gmp_printf("%6Qi", costs[0][i][j].score);
			
			if (j + 1 <= m)
				printf(", ");
		}
		
		puts("");
	}
	#endif
	
	fputs("score: ", stdout), mpq_print(costs[0][n][m].score), putchar('\n');
	
	unsigned k = 0;
	
	for (i = n, j = m; i && j; )
	{
		switch ((edits[k++] = costs[0][i][j].action))
		{
			case ek_insert: j--; break;
			
			case ek_update: i--, j--; break;
			
			case ek_match: i--, j--; break;
			
			case ek_within: i--, j--; break;
			
			case ek_delete: i--; break;
		}
	}
	
	for (i = 0; i <= n; i++)
		for (j = 0; j <= m; j++)
			mpq_clear(costs[0][i][j].score);
	
	free(costs);
	
	mpq_clear(a), mpq_clear(b);
	mpq_clear(q);
	
	EXIT;
	return k;
}










