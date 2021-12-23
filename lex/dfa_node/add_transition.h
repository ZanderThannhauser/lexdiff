
#include <wchar.h>

struct dfa_node;
struct memory_arena;

int dfa_node_add_transition(
	struct dfa_node* this,
	struct memory_arena* arena,
	wchar_t value,
	struct dfa_node* to);
