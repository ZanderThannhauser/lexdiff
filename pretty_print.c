
#include <stdio.h>

#include <debug.h>

#include <token.h>

#include <edit_kind.h>

#include <token_list/struct.h>

#include "pretty_print.h"

void pretty_print(
	struct token_list* mlist,
	struct token_list* clist,
	enum edit_kind* edits,
	unsigned number_of_edits)
{
	ENTER;
	
	struct pos {
		unsigned i, line;
	} m = {0, 1}, c = {0, 1};
	
	printf("\e[32m%3u\e[m/\e[31m%3u\e[0m | ", m.line, c.line);
	
	void escape(unsigned x, const char* str) {
		for (char h; (h = *str++); ) {
			switch (h) {
				case '\n':
					printf("↲\n");
					if (x & 1) c.line++;
					if (x & 2) m.line++;
					printf("\e[32m%3u\e[m/\e[31m%3u\e[0m | ", m.line, c.line);
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
				printf("\e[31m"), escape(1, clist->data[c.i]->data), printf("\e[0m"), c.i++;
				break;
			
			case ek_update:
/*				printf("\e[33m"), escape(3, mlist->data[m.i]->data), printf("\e[0m"), m.i++, c.i++;*/
				printf("\e[33m"), escape(3, clist->data[c.i]->data), printf("\e[0m"), m.i++, c.i++;
				break;
			
			case ek_match:
				printf("\e[38;2;180;180;180m"), escape(3, mlist->data[m.i]->data), printf("\e[0m"), m.i++, c.i++;
				break;
			
			case ek_delete:
				printf("\e[32m"), escape(2, mlist->data[m.i]->data), printf("\e[0m"), m.i++;
				break;
		}
	}
	
	puts("");
	
	EXIT;
}













