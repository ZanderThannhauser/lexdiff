
#include <debug.h>

#include "struct.h"
#include "set_match.h"

void token_rule_set_match(
	struct token_rule* this,
	mpq_ptr new_value)
{
	mpq_set(this->match, new_value);
}

