
#include <stdlib.h>

#include <debug.h>

#include "struct.h"
#include "free.h"

void free_id_to_cost_node(void* ptr)
{
	ENTER;
	
	struct id_to_cost_node *node = ptr;
	
	mpq_clear(node->insert), free(node->insert);
	mpq_clear(node->update), free(node->update);
	mpq_clear(node->match ), free(node->match );
	mpq_clear(node->delete), free(node->delete);
	
	free(node);
	
	EXIT;
}
