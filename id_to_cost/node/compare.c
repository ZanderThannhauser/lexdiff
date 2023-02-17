
#include <debug.h>

#include "struct.h"
#include "compare.h"

int compare_id_to_cost_node(
	const void* a,
	const void* b)
{
	const struct id_to_cost_node *A = a, *B = b;
	
	if (A->id > B->id)
		return +1;
	else if (A->id < B->id)
		return -1;
	else
		return +0;
}

