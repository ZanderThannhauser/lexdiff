
#include <debug.h>

#include <avl/tree.h>

#include "struct.h"
#include "compare.h"

int compare_regexsets(
	const struct regexset* a,
	const struct regexset* b)
{
	struct avl_node_t *an, *bn;
	
	for (an = a->tree->head, bn = b->tree->head;
		an && bn; an = an->next, bn = bn->next)
	{
		struct regex* ar = an->item, *br = bn->item;
		
		if (ar > br)
			return +1;
		else if (ar < br)
			return -1;
	}
	
	return an ? 1 : bn ? -1 : 0;
}
