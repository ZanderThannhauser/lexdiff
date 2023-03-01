
#include <debug.h>

#include "struct.h"
#include "set_update.h"

void token_rule_set_update(
	struct token_rule* this,
	mpq_ptr new_value)
{
	mpq_set(this->update, new_value);
}

