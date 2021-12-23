
enum token;

union tokendata;

struct memory_arena;
struct nfa_node;

int read_rule(
	int fd,
	uint8_t* current_byte,
	int (*read_byte)(int, uint8_t*),
	wchar_t* current_char,
	enum token* current_token,
	union tokendata* current_token_data,
	struct memory_arena* regex_arena,
	unsigned rule_id,
	struct nfa_node** out_node);
