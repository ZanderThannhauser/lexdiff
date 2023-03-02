
#include <debug.h>

#include <memory/srealloc.h>

#include <token/inc.h>

#include "struct.h"
#include "append.h"

void token_list_append(
	struct token_list* this,
	struct token* token)
{
	ENTER;
	
	if (this->n == this->cap)
	{
		this->cap = this->cap << 1 ?: 1;
		this->data = srealloc(this->data, sizeof(*this->data) * this->cap);
	}

	this->data[this->n++] = inc_token(token);
	
	EXIT;
}

