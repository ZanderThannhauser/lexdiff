
#include <stdlib.h>

#include <debug.h>

#include <quack/new.h>
#include <quack/append.h>
#include <quack/is_nonempty.h>
#include <quack/pop.h>
#include <quack/free.h>

#include <set/regex/new.h>
#include <set/regex/add.h>
#include <set/regex/free.h>

#include "struct.h"
#include "free.h"

void free_regex(
	struct regex* this)
{
	ENTER;
	
	struct quack* todo = new_quack();
	
	struct regexset* queued = new_regexset();
	
	regexset_add(queued, this);
	
	quack_append(todo, this);
	
	while (quack_is_nonempty(todo))
	{
		struct regex* regex = quack_pop(todo);
		
		for (unsigned i = 0; i < 256; i++)
		{
			struct regex* to = regex->transitions[i];
			
			if (to && regexset_add(queued, to))
				quack_append(todo, to);
		}
		
		for (unsigned i = 0, n = regex->lambdas.n; i < n; i++)
		{
			struct regex* to = regex->lambdas.data[i];
			
			if (to && regexset_add(queued, to))
				quack_append(todo, to);
		}
		
		free(regex->lambdas.data);
		
		free(regex);
	}
	
	free_regexset(queued);
	
	free_quack(todo);
	
	EXIT;
}



















