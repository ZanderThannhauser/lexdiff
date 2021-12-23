
#include <debug.h>

#include <memory/arena/realloc.h>

#include "struct.h"
#include "add_transition.h"

int dfa_node_add_transition(
	struct dfa_node* this,
	struct memory_arena* arena,
	wchar_t value,
	struct dfa_node* to)
{
	int error = 0;
	ENTER;
	
	dpv(value);
	
	if (this->transitions.n + 1 >= this->transitions.cap)
	{
		this->transitions.cap = this->transitions.cap * 2 ?: 1;
		
		error = arena_realloc(
			arena, (void**) &this->transitions.data,
			sizeof(*this->transitions.data) * this->transitions.cap);
	}
	
	if (!error)
	{
		this->transitions.data[this->transitions.n].value = value;
		this->transitions.data[this->transitions.n].to = to;
		
		this->transitions.n++;
	}
	
	
	EXIT;
	return error;
}

