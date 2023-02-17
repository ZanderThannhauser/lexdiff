
#include <assert.h>
#include <debug.h>

#include <memory/smalloc.h>

#include <avl/search.h>
#include <avl/insert.h>

#include "node/struct.h"

#include "struct.h"
#include "set.h"

void id_to_cost_set(
	struct id_to_cost* this,
	unsigned id,
	mpq_ptr insert,
	mpq_ptr update,
	mpq_ptr match,
	mpq_ptr delete)
{
	ENTER;
	
	assert(insert);
	assert(update);
	assert(match);
	assert(delete);
	
	struct avl_node_t* node = avl_search(this->tree, &id);
	
	if (node)
	{
		TODO;
	}
	else
	{
		struct id_to_cost_node* new = smalloc(sizeof(*new));
		
		new->id = id;
		new->insert = insert;
		new->update = update;
		new->match = match;
		new->delete = delete;
		
		avl_insert(this->tree, new);
	}
	
	EXIT;
}

