
#include <debug.h>

#include <memory/smalloc.h>

#include <avl/alloc_tree.h>

#include "node/compare.h"
#include "node/free.h"

#include "struct.h"
#include "new.h"

struct id_to_cost* new_id_to_cost()
{
	ENTER;
	
	struct id_to_cost* this = smalloc(sizeof(*this));
	
	this->tree = avl_alloc_tree(compare_id_to_cost_node, free_id_to_cost_node);
	
	EXIT;
	return this;
}

