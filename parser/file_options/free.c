
#include <debug.h>

#include "free.h"

void free_file_options(struct file_options* this)
{
	ENTER;
	
	free(this);
	
	EXIT;
}

