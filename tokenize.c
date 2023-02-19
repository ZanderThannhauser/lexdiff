
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <debug.h>

#include <defines/argv0.h>

#include <memory/smalloc.h>
#include <memory/srealloc.h>

#include <regex/struct.h>

#include <token_list/new.h>
#include <token_list/append.h>

#include "token.h"
#include "tokenize.h"

struct token_list* tokenize(FILE* stream, struct regex* tokenizer)
{
	ENTER;
	
	struct token_list* tlist = new_token_list();
	
	struct {
		char* data;
		unsigned n, cap;
	} buffer = {};
	
	unsigned line = 1, backup_line = 1, i = 0, accepts = 0;
	
	struct regex* state = tokenizer, *fallback = NULL;
	
	for (bool keep_going = !feof(stream); keep_going; )
	{
		int c;
		
		struct regex* to;
		
		if (i < buffer.n)
		{
			c = buffer.data[i];
			
			to = state->transitions[c];
		}
		else if ((c = fgetc(stream)) != EOF)
		{
			if (buffer.n == buffer.cap)
			{
				buffer.cap = buffer.cap << 1 ?: 1;
				buffer.data = realloc(buffer.data, sizeof(*buffer.data) * buffer.cap);
			}
			
			buffer.data[buffer.n++] = c;
			
			to = state->transitions[c];
		}
		else
		{
			to = NULL;
		}
		
		dpv(to);
		
		if (to)
		{
			if (state->accepts) fallback = state, accepts = state->accepts;
			
			if (c == '\n') line++;
			
			i++, state = to;
			
		}
		else if (state->accepts)
		{
			dpvsn(buffer.data, i);
			
			struct token* token = smalloc(sizeof(*token));
			
			token->id = state->accepts;
			
			token->data = memcpy(malloc(i + 1), buffer.data, i);
			token->data[i] = 0;
			
			token_list_append(tlist, token);
			
			state = tokenizer, fallback = NULL;
			
			memmove(buffer.data, buffer.data + i, buffer.n - i), buffer.n -= i, i = 0;
			
			backup_line = line;
			
			if (!buffer.n && feof(stream)) keep_going = false;
		}
		else if (fallback)
		{
			printf("accepts = %u\n", accepts);
			printf("backup_line = %u\n", backup_line);
			TODO;
		}
		else
		{
			fprintf(stderr, "%s: cannot tokenize '%.*s' on line %u!\n",
				argv0, buffer.n, buffer.data, line);
			exit(1);
		}
	}
	
	free(buffer.data);
	
	EXIT;
	return tlist;
}

















