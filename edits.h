
#include "kind.h"

struct edits
{
	enum edit_kind kind;
	
	struct edits* prev;
};
