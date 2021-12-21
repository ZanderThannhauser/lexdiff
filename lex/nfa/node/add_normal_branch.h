
struct nfa_node;
struct arena;

int nfa_node_add_normal_branch(
	struct nfa_node* this,
	struct arena* arena,
	wchar_t codepoint,
	struct nfa_node* branch);
