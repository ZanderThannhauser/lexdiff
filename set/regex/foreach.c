
#include <debug.h>

#include <avl/tree.h>

#include "struct.h"
#include "foreach.h"

void regexset_foreach(
	struct regexset* this,
	void (*callback)(struct regex*))
{
	ENTER;
	
	for (struct avl_node_t* node = this->tree->head; node; node = node->next)
		callback(node->item);
	
	EXIT;
}

