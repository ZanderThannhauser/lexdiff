
#include <stdio.h>

#include <debug.h>

#include <token.h>

#include <edit_kind.h>

#include <token_list/struct.h>

#include "pretty_print.h"

void pretty_print(
	struct token_list* blist,
	struct token_list* alist,
	enum edit_kind* edits,
	unsigned number_of_edits)
{
	ENTER;
	
	struct pos {
		unsigned i, line;
	} b = {0, 1}, a = {0, 1};
	
	printf("\e[32m%3u\e[m/\e[31m%3u\e[0m | ", b.line, a.line);
	
	void escape(unsigned x, const char* str) {
		for (char h; (h = *str++); ) {
			switch (h) {
				case '\n':
					printf("↲\n");
					if (x & 1) a.line++;
					if (x & 2) b.line++;
					printf("\e[32m%3u\e[m/\e[31m%3u\e[0m | ", b.line, a.line);
					break;
				
				case '\t':
					printf("↦\t");
					break;
				
				case ' ':
					printf("␣");
					break;
				
				default:
					fputc(h, stdout);
					break;
			}
		}
	}

	for (unsigned e = number_of_edits; e > 0; e--)
	{
		switch (edits[e - 1])
		{
			case ek_insert:
				printf("\e[32m"), escape(1, alist->data[a.i]->data), printf("\e[0m"), a.i++;
				break;
			
			case ek_update:
/*				printf("\e[33m"), escape(3, mlist->data[m.i]->data), printf("\e[0m"), b.i++, a.i++;*/
				printf("\e[33m"), escape(3, alist->data[a.i]->data), printf("\e[0m"), b.i++, a.i++;
				break;
			
			case ek_match:
				printf("\e[38;2;180;180;180m"), escape(3, blist->data[b.i]->data), printf("\e[0m"), b.i++, a.i++;
				break;
			
			case ek_delete:
				printf("\e[31m"), escape(2, blist->data[b.i]->data), printf("\e[0m"), b.i++;
				break;
		}
	}
	
	puts("");
	
	EXIT;
}













