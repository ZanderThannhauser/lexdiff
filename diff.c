
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <gmp.h>

#include <memory/smalloc.h>

#include <debug.h>

#include <id_to_cost/get.h>

#include <cmdln/verbose.h>

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

unsigned diff(
	struct id_to_cost* idtoc,
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
		
		mpq_add(q, q, id_to_cost_get_delete(idtoc, before->data[i]->id));
		
		cell->action = ek_delete;
		mpq_init_set(cell->score, q);
	}
	
	mpq_set_ui(q, 0, 1);
	
	for (j = 0, m = after->n; j < m; j++)
	{
		struct cell* cell = &costs[0][0][j+1];
		
		mpq_add(q, q, id_to_cost_get_insert(idtoc, after->data[j]->id));
		
		cell->action = ek_insert;
		mpq_init_set(cell->score, q);
	}
	
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			unsigned mid = before->data[i]->id;
			unsigned cid = after->data[j]->id;
			
			struct cell* cell = &costs[0][i+1][j+1];
			
			mpq_init(cell->score);
			
			// consider delete:
			mpq_add(cell->score, costs[0][i][j+1].score, id_to_cost_get_delete(idtoc, mid));
			cell->action = ek_delete;
			
			// consider insert:
			mpq_add(q, costs[0][i+1][j].score, id_to_cost_get_insert(idtoc, cid));
			
			if (mpq_cmp(q, cell->score) > 0)
			{
				mpq_set(cell->score, q);
				cell->action = ek_insert;
			}
			
			// if the tokens are of a different kind, that's too different
			if (mid == cid)
			{
				mpq_ptr delta;
				
				enum edit_kind action;
				
				if (strcmp(before->data[i]->data, after->data[j]->data))
				{
					delta = id_to_cost_get_update(idtoc, cid);
					action = ek_update;
				}
				else
				{
					delta = id_to_cost_get_match(idtoc, cid);
					action = ek_match;
				}
				
				// consider change:
				mpq_add(q, costs[0][i][j].score, delta);
				
				if (mpq_cmp(q, cell->score) >= 0)
				{
					mpq_set(cell->score, q);
					cell->action = action;
				}
			}
		}
	}
	
	fputs("score: ", stdout), mpq_print(costs[0][n][m].score), putchar('\n');
	
	unsigned k = 0;
	
	for (i = n, j = m; i && j; )
	{
		switch ((edits[k++] = costs[0][i][j].action))
		{
			case ek_insert: j--; break;
			
			case ek_update: i--, j--; break;
			
			case ek_match: i--, j--; break;
			
			case ek_delete: i--; break;
		}
	}
	
	for (i = 0; i <= n; i++)
		for (j = 0; j <= m; j++)
			mpq_clear(costs[0][i][j].score);
	
	free(costs);
	
	mpq_clear(q);
	
	EXIT;
	return k;
}














