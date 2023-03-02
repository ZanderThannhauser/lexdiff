
#include <stdlib.h>
#include <assert.h>

#include <debug.h>

#include <token_rule/free.h>

#include "struct.h"
#include "free.h"

void free_token(
	struct token* this)
{
	ENTER;
	
	if (this && !--this->refcount)
	{
		free_token_rule(this->rule);
		
		mpq_clear(this->value);
		
		free(this->chars);
		free(this);
	}
	
	EXIT;
}

