
struct token_rule;

struct token* new_token(
	struct token_rule* rule,
	const char* chars,
	unsigned len);
