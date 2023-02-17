
#include <debug.h>

#include <memory/smalloc.h>

#include "struct.h"
#include "new.h"

struct token_list* new_token_list()
{
	ENTER;
	
	struct token_list* this = smalloc(sizeof(*this));
	
	this->data = NULL;
	this->n = 0;
	this->cap = 0;
	
	EXIT;
	return this;
}
