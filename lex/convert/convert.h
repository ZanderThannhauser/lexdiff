
struct nfa_node;
struct dfa_node;
struct memory_arena;

int convert_nfa_to_dfa(
	struct nfa_node* in,
	struct memory_arena* ,
	struct memory_arena* ,
	struct dfa_node** out);
