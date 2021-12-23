
#include <debug.h>

#include <macros/min.h>
#include <macros/cmp.h>

#include "statelist/struct.h"

#include "compare_bundles.h"

int compare_bundles(const void* a, const void* b)
{
	int cmp = 0;
	size_t i, n;
	struct statelist * const *A = a, * const *B = b;
	ENTER;
	
	if (*A != *B)
	{
		cmp = cmp((*A)->n, (*B)->n);
		
		for (i = 0, n = min((*A)->n, (*B)->n); !cmp && i < n; i++)
			cmp = cmp((*A)->states[i], (*B)->states[i]);
	}
	
	EXIT;
	return cmp;
}
