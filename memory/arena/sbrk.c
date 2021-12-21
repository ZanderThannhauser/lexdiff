
#include <sys/mman.h>

#include <valgrind/memcheck.h>

#include <debug.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include "struct.h"
#include "sbrk.h"

int arena_sbrk(
	struct arena* this,
	void** out,
	size_t size)
{
	int error = 0;
	ENTER;
	
	dpv(size);
	
	assert(size);
	
	void* ptr = this->start + this->cap;
	
	void* old_start = this->start;
	size_t old_cap = this->cap;
	
	void* new_start;
	size_t new_cap = this->cap + size;
	
	if ((new_start = mremap(old_start, old_cap, new_cap, 0)) == MAP_FAILED)
	{
		TODO;
		error = 1;
	}
	else
	{
		this->start = new_start;
		this->cap = new_cap;
		
		dpv(this->start);
		dpv(this->cap);
		
		*out = ptr;
	}
	
	EXIT;
	return error;
}













