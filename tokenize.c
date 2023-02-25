
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
	
	unsigned line = 1, fallback_line;
	unsigned i = 0, fallback_i;
	
	struct regex* state = tokenizer, *fallback = NULL;
	
	int c;
	struct regex* to;
	
	for (bool keep_going = !feof(stream); keep_going; )
	{
		if (i < buffer.n)
		{
			c = buffer.data[i];
			
			ddputs("reading from buffer");
			
			dpvc(c);
			
			to = state->transitions[c];
		}
		else
		{
			ddputs("reading from stream");
			
			c = fgetc(stream);
			
			if (c == EOF)
			{
				ddputs("hit EOF");
				to = NULL;
			}
			else
			{
				dpvc(c);
				
				if (buffer.n == buffer.cap)
				{
					buffer.cap = buffer.cap << 1 ?: 1;
					buffer.data = realloc(buffer.data, sizeof(*buffer.data) * buffer.cap);
				}
				
				buffer.data[buffer.n++] = c;
				
				dpvsn(buffer.data, buffer.n);
				
				to = state->transitions[c];
			}
		}
		
		if (to)
		{
			dpv(to);
			
			if (state->accepts)
			{
				dpv(state->accepts);
				fallback = state;
				fallback_i = i;
				fallback_line = line;
				ddprintf("fallback_i == %u\n", fallback_i);
				ddprintf("fallback_line == %u\n", fallback_line);
			}
			
			if (c == '\n') line++;
			
			i++, state = to;
			
			dpv(i);
		}
		else if (state->accepts)
		{
			dpv(state->accepts);
			
			ddprintf("token: \"%.*s\"\n", i, buffer.data);
			
			struct token* token = smalloc(sizeof(*token));
			
			token->id = state->accepts;
			
			token->data = memcpy(malloc(i + 1), buffer.data, i);
			token->data[i] = 0;
			
			token_list_append(tlist, token);
			
			state = tokenizer, fallback = NULL;
			
			memmove(buffer.data, buffer.data + i, buffer.n - i), buffer.n -= i, i = 0;
			
			dpvsn(buffer.data, buffer.n);
			dpv(i);
			
			if (!buffer.n && feof(stream)) keep_going = false;
		}
		else if (fallback)
		{
			ddprintf("falling back to: i = %u, line = %u\n", fallback_i, fallback_line);
			
			i = fallback_i, line = fallback_line;
			
			ddprintf("(fallback) token: \"%.*s\"\n", i, buffer.data);
			
			struct token* token = smalloc(sizeof(*token));
			
			token->id = fallback->accepts;
			
			token->data = memcpy(malloc(i + 1), buffer.data, i);
			token->data[i] = 0;
			
			token_list_append(tlist, token);
			
			state = tokenizer, fallback = NULL;
			
			memmove(buffer.data, buffer.data + i, buffer.n - i), buffer.n -= i, i = 0;
			
			dpvsn(buffer.data, buffer.n);
			dpv(i);
		}
		else
		{
			fprintf(stderr, "%s: unexpected '%c' when reading '%.*s' on line %u!\n",
				argv0, c, i, buffer.data, line);
			exit(1);
		}
	}
	
	free(buffer.data);
	
	EXIT;
	return tlist;
}

















