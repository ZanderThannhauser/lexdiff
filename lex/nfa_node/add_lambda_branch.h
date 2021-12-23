
struct nfa_node;
struct memory_arena;

int nfa_node_add_lambda_branch(
	struct nfa_node* this,
	struct memory_arena* arena,
	struct nfa_node* node);
