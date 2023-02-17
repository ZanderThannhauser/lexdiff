
#include <assert.h>
#include <debug.h>

#include <avl/tree.h>

#include "struct.h"
#include "get_head.h"

struct regex* regexset_get_head(
	const struct regexset* this)
{
	assert(this->tree->head);
	return this->tree->head->item;
}

