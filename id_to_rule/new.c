
#include <debug.h>

#include <memory/smalloc.h>

#include <avl/alloc_tree.h>

#include "node/compare.h"
#include "node/free.h"

#include "struct.h"
#include "new.h"

struct id_to_rule* new_id_to_rule()
{
	ENTER;
	
	struct id_to_rule* this = smalloc(sizeof(*this));
	
	this->tree = avl_alloc_tree(compare_id_to_rule_node, free_id_to_rule_node);
	
	EXIT;
	return this;
}

