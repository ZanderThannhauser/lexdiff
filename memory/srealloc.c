
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <debug.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include "srealloc.h"

void* srealloc(void* ptr, size_t newsize)
{
	void* newptr = realloc(ptr, newsize);
	
	if (!newptr)
	{
		fprintf(stderr, "%s: realloc(%lu): %m\n", argv0, newsize);
		exit(e_out_of_memory);
	}
	
	return newptr;
}

