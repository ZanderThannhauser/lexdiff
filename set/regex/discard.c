
#include <debug.h>

#include <avl/delete.h>

#include "struct.h"
#include "discard.h"

void regexset_discard(
	struct regexset* this,
	struct regex* ele)
{
	avl_delete(this->tree, ele);
}

