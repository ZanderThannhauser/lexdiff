
#include <assert.h>

#include <debug.h>

#include <avl/search.h>

#include "node/struct.h"

#include "struct.h"
#include "get.h"

struct token_rule* id_to_rule_get_rule(
	struct id_to_rule* this,
	unsigned id)
{
	struct avl_node_t* node = avl_search(this->tree, &id);
	struct id_to_rule_node* x = node->item;
	return x->rule;
}




















