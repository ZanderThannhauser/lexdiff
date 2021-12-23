
#include <debug.h>

#include <macros/cmp.h>

#include "compare.h"

int compare_states(const void* a, const void* b)
{
	int cmp = 0;
	ENTER;
	
	dpv(*((void**) a));
	dpv(*((void**) b));
	
/*	dpv(cmp(((void**) a)[0], ((void**) b)[0]));*/
	
	cmp = cmp(((void**) a)[0], ((void**) b)[0]);
	
	EXIT;
	return cmp;
}
