
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include <debug.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include <avl/insert.h>

#include "struct.h"
#include "add.h"

bool regexset_add(
	struct regexset* this,
	struct regex* regex)
{
	bool new;
	ENTER;
	
	errno = 0;
	
	struct avl_node_t* node = avl_insert(this->tree, regex);
	
	if (node)
		new = true;
	else if (errno == EEXIST)
		new = false;
	else
	{
		fprintf(stderr, "%s: regexset_add: %m\n", argv0);
		exit(e_syscall_failed);
	}
	
	EXIT;
	return new;
}

