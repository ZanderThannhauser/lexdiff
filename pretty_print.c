
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <debug.h>

#include <edit_kind.h>

#include <token_list/struct.h>

#include <cmdln/width.h>

#include <token/struct.h>

#include <mpq_print.h>

#include <token_rule/struct.h>

#include "diff_cell.h"
#include "print_token.h"
#include "pretty_print.h"

void pretty_print(
	struct diff_cell* _costs,
	struct token_list* btoks,
	struct token_list* atoks)
{
	ENTER;
	
	unsigned k = 0, i, j, n = btoks->n, m = atoks->n;
	
	struct diff_cell (*const costs)[n+1][m+1] = (void*) _costs;
	
	enum edit_kind* edits = malloc(sizeof(*edits) * (n + m));
	
	ddputs("backtracking:");
	for (i = n, j = m; i && j; )
	{
		switch ((edits[k++] = costs[0][i][j].action))
		{
			case ek_insert:
			{
				ddputs("case ek_insert:");
				j--;
				break;
			}
			
			case ek_update:
			{
				ddputs("case ek_update:");
				i--, j--;
				break;
			}
			
			case ek_match:
			{
				ddputs("case ek_match:");
				i--, j--;
				break;
			}
			
			case ek_within:
			{
				ddputs("case ek_within:");
				i--, j--;
				break;
			}
			
			case ek_delete:
			{
				ddputs("case ek_delete:");
				i--;
				break;
			}
		}
	}
	
	ddputs("pretty printing:");
	
	bool color = isatty(1);
	
	unsigned bidx   = 0, aidx  = 0;
	unsigned bline  = 1, aline = 1;
	unsigned btok   = 0, atok  = 0;
	unsigned btok2  = 0, atok2 = 0;
	signed e = k, f = k + 1;
	
	while (e > 0)
	{
		unsigned col = printf("%3u/%3u | ", bline, aline);
		
		while (e > 0)
		{
			switch (edits[e-1])
			{
				case ek_insert:
				{
					ddputs("case ek_insert:");
					
					if (color) printf("\e[38;2;100;255;100m");
					
					char* data = atoks->data[atok]->chars + aidx;
					unsigned len = strcspn(data, "\n");
					
					if (data[len])
					{
						print_token(stdout, data, ++len), aline++, aidx += len, col += len;
						goto newline;
					}
					else
					{
						print_token(stdout, data, len), atok++, aidx = 0, col += len;
						break;
					}
				}
				
				case ek_match:
				{
					ddputs("case ek_match:");
					
					if (color) printf("\e[38;2;100;100;100m");
					
					char* data = atoks->data[atok]->chars + aidx;
					unsigned len = strcspn(data, "\n");
					
					if (data[len])
					{
						print_token(stdout, data, ++len), bline++, aline++, aidx += len, col += len;
						goto newline;
					}
					else
					{
						print_token(stdout, data, len), btok++, atok++, bidx = 0, aidx = 0, col += len;
						break;
					}
				}
				
				case ek_update:
				{
					ddputs("case ek_update:");
					
					if (color) printf("\e[38;2;255;255;100m");
					
					char* data = atoks->data[atok]->chars + aidx;
					unsigned len = strcspn(data, "\n");
					
					if (data[len])
					{
						print_token(stdout, data, ++len), bline++, aline++, aidx += len, col += len;
						goto newline;
					}
					else
					{
						print_token(stdout, data, len), btok++, atok++, bidx = 0, aidx = 0, col += len;
						break;
					}
				}
				
				case ek_within:
				{
					ddputs("case ek_within:");
					
					if (color) printf("\e[38;2;255;100;255m");
					
					char* data = atoks->data[atok]->chars + aidx;
					unsigned len = strcspn(data, "\n");
					
					if (data[len])
					{
						print_token(stdout, data, ++len), bline++, aline++, aidx += len, col += len;
						goto newline;
					}
					else
					{
						print_token(stdout, data, len), btok++, atok++, bidx = 0, aidx = 0, col += len;
						break;
					}
				}
				
				case ek_delete:
				{
					ddputs("case ek_delete:");
					
					if (color) printf("\e[38;2;255;100;100m");
					
					char* data = btoks->data[btok]->chars + bidx;
					unsigned len = strcspn(data, "\n");
					
					if (data[len])
					{
						print_token(stdout, data, ++len), bline++, bidx += len, col += len;
						goto newline;
					}
					else
					{
						print_token(stdout, data, len), btok++, bidx = 0, col += len;
						break;
					}
				}
			}
			
			e--;
		}
		
		newline: if (color) printf("\e[0m");
		
		bool newline = true;
		
		while (f > 1 && f != e)
		{
			mpq_ptr delta = costs[0][btok2+1][atok2+1].delta;
			
			switch (edits[--f-1])
			{
				case ek_insert:
				{
					if (mpq_sgn(delta))
					{
						printf("%*s| extra '", width - col, "");
						print_token(stdout, atoks->data[atok2]->chars, -1);
						printf("' ("), mpq_print(delta), printf(")\n");
						col = 0, newline = false;
					}
					atok2++;
					break;
				}
				
				case ek_match:
				{
					if (mpq_sgn(delta))
					{
						printf("%*s| exact match of '", width - col, "");
						print_token(stdout, atoks->data[atok2]->chars, -1);
						printf("' ("), mpq_print(delta), printf(")\n");
						col = 0, newline = false;
					}
					btok2++, atok2++;
					break;
				}
				
				case ek_update:
				{
					if (mpq_sgn(delta))
					{
						printf("%*s| '", width - col, "");
						print_token(stdout, atoks->data[atok2]->chars, -1);
						printf("' instead of '");
						print_token(stdout, btoks->data[btok2]->chars, -1);
						printf("' ("), mpq_print(delta), printf(")\n");
						col = 0, newline = false;
					}
					
					btok2++, atok2++;
					break;
				}
				
				case ek_within:
				{
					if (mpq_sgn(delta))
					{
						printf("%*s| ", width - col, "");
						print_token(stdout, atoks->data[atok2]->chars, -1);
						printf(" numerically close enough to ");
						print_token(stdout, btoks->data[btok2]->chars, -1);
						printf(" ("), mpq_print(delta), printf(")\n");
						col = 0, newline = false;
					}
					
					btok2++, atok2++;
					break;
				}
				
				case ek_delete:
				{
					if (mpq_sgn(delta))
					{
						printf("%*s| missing '", width - col, "");
						print_token(stdout, btoks->data[btok2]->chars, -1);
						printf("' ("), mpq_print(delta), printf(")\n");
						col = 0, newline = false;
					}
					
					btok2++;
					break;
				}
			}
		}
		
		if (newline)
			puts("");
	}
	
	free(edits);
	
	EXIT;
}













