
#include <stdlib.h>
#include <debug.h>

#include <avl/tree.h>
#include <avl/free_tree.h>

#include <regex/free.h>

#include "struct.h"
#include "free.h"

void free_regexset(
	struct regexset* this)
{
	if (!--this->refcount)
	{
		avl_free_tree(this->tree);
		
		free(this);
	}
}

void free_regexset_and_elements(
	struct regexset* this)
{
	if (!--this->refcount)
	{
		for (struct avl_node_t* node = this->tree->head; node; node = node->next)
			free_regex(node->item);
		
		avl_free_tree(this->tree);
		
		free(this);
	}
}

