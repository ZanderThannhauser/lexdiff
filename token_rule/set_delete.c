
#include <debug.h>

#include "struct.h"
#include "set_delete.h"

void token_rule_set_delete(
	struct token_rule* this,
	mpq_ptr new_value)
{
	mpq_set(this->delete, new_value);
}

