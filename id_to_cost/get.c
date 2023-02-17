
#include <assert.h>

#include <debug.h>

#include <avl/search.h>

#include "node/struct.h"
#include "struct.h"
#include "get.h"

mpq_ptr id_to_cost_get_insert(
	struct id_to_cost* this,
	unsigned id)
{
	struct avl_node_t* node = avl_search(this->tree, &id);
	assert(node);
	struct id_to_cost_node* x = node->item;
	return x->insert;
}

mpq_ptr id_to_cost_get_update(
	struct id_to_cost* this,
	unsigned id)
{
	struct avl_node_t* node = avl_search(this->tree, &id);
	assert(node);
	struct id_to_cost_node* x = node->item;
	return x->update;
}

mpq_ptr id_to_cost_get_match(
	struct id_to_cost* this,
	unsigned id)
{
	struct avl_node_t* node = avl_search(this->tree, &id);
	assert(node);
	struct id_to_cost_node* x = node->item;
	return x->match;
}

mpq_ptr id_to_cost_get_delete(
	struct id_to_cost* this,
	unsigned id)
{
	struct avl_node_t* node = avl_search(this->tree, &id);
	assert(node);
	struct id_to_cost_node* x = node->item;
	return x->delete;
}



















