
#include <debug.h>

#include <avl/search.h>

#include "struct.h"
#include "contains.h"

bool regexset_contains(
	const struct regexset* this,
	struct regex* ele)
{
	return !!avl_search(this->tree, ele);
}

