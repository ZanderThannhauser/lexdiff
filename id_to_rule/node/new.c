
#include <debug.h>

#include <memory/smalloc.h>

#include <token_rule/inc.h>

#include "struct.h"
#include "new.h"

struct id_to_rule_node* new_id_to_rule_node(
	unsigned id,
	struct token_rule* rule)
{
	ENTER;
	
	struct id_to_rule_node* new = smalloc(sizeof(*new));
	
	new->id = id;
	
	new->rule = inc_token_rule(rule);
	
	EXIT;
	return new;
}

