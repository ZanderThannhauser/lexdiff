
#include <debug.h>

#include "struct.h"
#include "inc.h"

struct token* inc_token(
	struct token* this)
{
	if (this)
		this->refcount++;
	return this;
}

