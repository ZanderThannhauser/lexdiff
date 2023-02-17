
#include <debug.h>

#include <avl/tree.h>

#include "struct.h"
#include "is_nonempty.h"

bool regexset_is_nonempty(
	const struct regexset* this)
{
	return !!this->tree->head;
}

