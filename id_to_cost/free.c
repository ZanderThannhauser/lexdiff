
#include <stdlib.h>

#include <debug.h>

#include <avl/free_tree.h>

#include "struct.h"
#include "free.h"

void free_id_to_cost(
	struct id_to_cost* this)
{
	ENTER;
	
	avl_free_tree(this->tree);
	
	free(this);
	
	EXIT;
}

