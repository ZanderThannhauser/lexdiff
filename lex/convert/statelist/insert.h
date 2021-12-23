
struct memory_arena;
struct statelist;
struct nfa_node;

int statelist_insert(struct statelist* this,
	struct memory_arena*,
	struct nfa_node* nfa_node);
