
#include <wchar.h>

struct nfa_node;
struct memory_arena;

int nfa_node_add_normal_branch(
	struct nfa_node* this,
	struct memory_arena* arena,
	wchar_t codepoint,
	struct nfa_node* branch);
