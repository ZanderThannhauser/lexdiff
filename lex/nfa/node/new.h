
struct nfa_node;
struct arena;

int new_nfa_node(
	struct nfa_node** out,
	struct arena* arena);
