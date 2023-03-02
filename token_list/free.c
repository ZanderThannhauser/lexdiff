
#include <assert.h>
#include <stdlib.h>

#include <debug.h>

#include <token/free.h>

#include "struct.h"
#include "free.h"

void free_token_list(
	struct token_list* this)
{
	ENTER;
	
	for (unsigned i = 0, n = this->n; i < n; i++)
	{
		free_token(this->data[i]);
	}
	
	free(this->data);
	free(this);
	
	EXIT;
}

