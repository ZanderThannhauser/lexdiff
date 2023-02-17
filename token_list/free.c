
#include <stdlib.h>

#include <debug.h>

#include <token.h>

#include "struct.h"
#include "free.h"

void free_token_list(
	struct token_list* this)
{
	ENTER;
	
	for (unsigned i = 0, n = this->n; i < n; i++)
	{
		struct token* token = this->data[i];
		
		free(token->data);
		free(token);
	}
	
	free(this->data);
	free(this);
	
	EXIT;
}

