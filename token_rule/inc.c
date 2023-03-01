
#include <debug.h>

#include "struct.h"
#include "inc.h"

struct token_rule* inc_token_rule(
	struct token_rule* this)
{
	if (this)
		this->refcount++;
	return this;
}

