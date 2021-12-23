
struct avl_tree_t;
struct statelist;
struct dfa_node;
struct memory_arena;

int combine_states(
	struct avl_tree_t* tree,
	struct statelist* statelist,
	struct memory_arena* throwaway_arena,
	struct memory_arena* dfa_arena,
	struct dfa_node** retval);
