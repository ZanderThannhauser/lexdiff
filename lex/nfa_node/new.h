
struct nfa_node;
struct memory_arena;

int new_nfa_node(
	struct nfa_node** out,
	struct memory_arena* arena);
