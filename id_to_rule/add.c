
#include <stdlib.h>
#include <assert.h>
#include <debug.h>

#include <avl/search.h>
#include <avl/insert.h>

#include "node/new.h"

#include "struct.h"
#include "add.h"

void id_to_rule_add(
	struct id_to_rule* this,
	unsigned id,
	struct token_rule* rule)
{
	ENTER;
	
	struct avl_node_t* node = avl_search(this->tree, &id);
	
	assert(!node);
	
	struct id_to_rule_node* itor = new_id_to_rule_node(id, rule);
	
	avl_insert(this->tree, itor);
	
	EXIT;
}




















