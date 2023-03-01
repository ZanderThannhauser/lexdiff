
struct token_rule;

struct id_to_rule;

void id_to_rule_add(
	struct id_to_rule* this,
	unsigned id,
	struct token_rule* token_rule);
