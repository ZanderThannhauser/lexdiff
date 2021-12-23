
struct nfa_node;
struct arena;
struct memory_arena;

int nfa_node_set_default_branch(
	struct nfa_node* this,
	struct memory_arena* arena,
	struct nfa_node* branch);
