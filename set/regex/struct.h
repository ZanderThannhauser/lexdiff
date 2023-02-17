
struct avl_tree_t;

struct regexset
{
	struct avl_tree_t* tree;
	
	unsigned refcount;
};

