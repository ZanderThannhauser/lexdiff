
struct nfa_node;
struct arena;

int nfa_node_add_default_branch(
	struct nfa_node* this,
	struct arena* arena,
	struct nfa_node* branch);
