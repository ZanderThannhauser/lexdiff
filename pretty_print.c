
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <debug.h>

#include <token.h>

#include <edit_kind.h>

#include <token_list/struct.h>

#include <cmdln/width.h>

#include <id_to_cost/get.h>

#include <mpq_print.h>

#include "pretty_print.h"

static void escape(char* moving, unsigned len)
{
	char c;
	while (len-- && (c = *moving++))
	{
		switch (c)
		{
			case '\n': printf("↲"); break;
			case '\t': printf("↦"); break;
			case  ' ': printf("␣"); break;
			default  : putchar(c); break;
		}
	}
}

void pretty_print(
	struct id_to_cost* idtoc,
	struct token_list* btoks,
	struct token_list* atoks,
	enum edit_kind* edits,
	unsigned number_of_edits)
{
	ENTER;
	
	unsigned bidx   = 0, aidx  = 0;
	unsigned bline  = 1, aline = 1;
	unsigned btok   = 0, atok  = 0;
	unsigned btok2  = 0, atok2 = 0;
	signed e = number_of_edits, f = number_of_edits + 1;
	
	while (e > 0)
	{
		unsigned col = printf("%3u/%3u | ", bline, aline);
		
		while (e > 0)
		{
			switch (edits[e-1])
			{
				case ek_insert:
				{
					printf("\e[38;2;100;255;100m");
					
					char* data = atoks->data[atok]->data + aidx;
					unsigned len = strcspn(data, "\n");
					
					if (data[len])
					{
						escape(data, ++len), aline++, aidx += len, col += len;
						goto newline;
					}
					else
					{
						escape(data, len), atok++, aidx = 0, col += len;
						break;
					}
				}
				
				case ek_match:
				{
					printf("\e[38;2;100;100;100m");
					
					char* data = atoks->data[atok]->data + aidx;
					unsigned len = strcspn(data, "\n");
					
					if (data[len])
					{
						escape(data, ++len), bline++, aline++, aidx += len, col += len;
						goto newline;
					}
					else
					{
						escape(data, len), btok++, atok++, bidx = 0, aidx = 0, col += len;
						break;
					}
				}
				
				case ek_update:
				{
					printf("\e[38;2;255;255;100m");
					
					char* data = atoks->data[atok]->data + aidx;
					unsigned len = strcspn(data, "\n");
					
					if (data[len])
					{
						escape(data, ++len), bline++, aline++, aidx += len, col += len;
						goto newline;
					}
					else
					{
						escape(data, len), btok++, atok++, bidx = 0, aidx = 0, col += len;
						break;
					}
				}
				
				case ek_delete:
				{
					printf("\e[38;2;255;100;100m");
					
					char* data = btoks->data[btok]->data + bidx;
					unsigned len = strcspn(data, "\n");
					
					if (data[len])
					{
						escape(data, ++len), bline++, bidx += len, col += len;
						goto newline;
					}
					else
					{
						escape(data, len), btok++, bidx = 0, col += len;
						break;
					}
				}
			}
			
			e--;
		}
		
		newline: printf("\e[0m");
		
		bool newline = true;
		
		while (f > 1 && f != e)
		{
			switch (edits[--f-1])
			{
				case ek_insert:
				{
					printf("%*s| extra '", width - col, "");
					escape(atoks->data[atok2]->data, -1);
					printf("' (");
					mpq_print(id_to_cost_get_insert(idtoc, atoks->data[atok2]->id));
					printf(")\n");
					col = 0, newline = false, atok2++;
					break;
				}
				
				case ek_match:
				{
					mpq_ptr m = id_to_cost_get_match(idtoc, atoks->data[atok2]->id);
					
					if (mpq_sgn(m))
					{
						printf("%*s| exact match '", width - col, "");
						escape(atoks->data[atok2]->data, -1);
						printf("' (");
						mpq_print(id_to_cost_get_match(idtoc, atoks->data[atok2]->id));
						printf(")\n");
						col = 0, newline = false;
					}
					
					btok2++, atok2++;
					break;
				}
				
				case ek_update:
				{
					printf("%*s| '", width - col, "");
					escape(btoks->data[btok2]->data, -1);
					printf("' instead of '");
					escape(atoks->data[atok2]->data, -1);
					printf("' (");
					mpq_print(id_to_cost_get_update(idtoc, btoks->data[btok2]->id));
					printf(")\n");
					col = 0, newline = false, btok2++, atok2++;
					break;
				}
				
				case ek_delete:
				{
					printf("%*s| missing '", width - col, "");
					escape(btoks->data[btok2]->data, -1);
					printf("' (");
					mpq_print(id_to_cost_get_insert(idtoc, btoks->data[btok2]->id));
					printf(")\n");
					col = 0, newline = false, btok2++;
					break;
				}
			};
		}
		
		if (newline)
			puts("");
	}
	
	EXIT;
}













