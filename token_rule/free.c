
#include <stdlib.h>

#include <debug.h>

#include "struct.h"
#include "free.h"

void free_token_rule(
	struct token_rule* this)
{
	ENTER;
	
	if (this && !--this->refcount)
	{
		mpq_clear(this->insert);
		mpq_clear(this->match);
		mpq_clear(this->update);
		mpq_clear(this->delete);
		
		for (unsigned i = 0, n = this->withins.n; i < n; i++)
		{
			mpq_clear(this->withins.data[i].tolerance);
			mpq_clear(this->withins.data[i].points);
		}
		
		free(this->withins.data);
		
		free(this);
	}
	
	EXIT;
}

