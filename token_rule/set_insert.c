
#include <debug.h>

#include "struct.h"
#include "set_insert.h"

void token_rule_set_insert(
	struct token_rule* this,
	mpq_ptr new_value)
{
	mpq_set(this->insert, new_value);
}

