
#include <stdlib.h>

#include <debug.h>

#include <token_rule/free.h>

#include "struct.h"
#include "free.h"

void free_id_to_rule_node(void* ptr)
{
	ENTER;
	
	struct id_to_rule_node *node = ptr;
	
	free_token_rule(node->rule);
	
	free(node);
	
	EXIT;
}
