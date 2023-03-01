
#include <gmp.h>

struct token_rule;

void token_rule_add_within(
	struct token_rule* this,
	mpq_t tolerance,
	bool is_percentage,
	mpq_t points);
