
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
	
	unsigned line = 1, i = 0;
	
	int c = fgetc(stream);
	
	struct regex* state = tokenizer, *fallback = NULL;
	
	for (bool keep_going = c != EOF; keep_going; )
	{
		dpvc(c);
		
		// if there's more in the buffer, transition on it
		// if not, read from the stream
		// if EOF: to = NULL;
		
		TODO;
		#if 0
		struct regex* to = c == EOF ? NULL : state->transitions[c];
		
		if (to)
		{
			if (state->accepts)
			{
				// remember fallback
				TODO;
			}
			else
			{
				TODO;
			}
			TODO;
			#if 0
			if (buffer.n == buffer.cap)
			{
				buffer.cap = buffer.cap << 1 ?: 1;
				buffer.data = srealloc(buffer.data, sizeof(*buffer.data) * buffer.cap);
			}
			
			state = to, buffer.data[buffer.n++] = c;
			
			if (c == '\n') line++;
			
			c = fgetc(stream);
			#endif
		}
		else if (state->accepts)
		{
			// memmove buffer back
			
			TODO;
			#if 0
			dpvsn(buffer.data, buffer.n);
			
			struct token* token = smalloc(sizeof(*token));
			
			token->id = state->accepts;
			
			token->data = memcpy(malloc(buffer.n + 1), buffer.data, buffer.n);
			token->data[buffer.n] = 0;
			
			token_list_append(tlist, token);
			
			state = tokenizer, buffer.n = 0, fallback = NULL;
			
			if (c == EOF) keep_going = false;
			#endif
		}
		else if (fallback)
		{
			TODO;
		}
		else
		{
			fprintf(stderr, "%s: cannot tokenize '%.*s%c' on line %u!\n",
				argv0, buffer.n, buffer.data, c, line);
			exit(1);
		}
		#endif
	}
	
	free(buffer.data);
	
	EXIT;
	return tlist;
}

















