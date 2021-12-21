
#include <sys/mman.h>
#include <valgrind/memcheck.h>

#include <debug.h>

#include <enums/error.h>

#include <defines/argv0.h>

#include <memory/smalloc.h>

#include "defines/INITIAL_MMAP_LENGTH.h"

#include "header.h"
#include "footer.h"
#include "struct.h"
#include "new.h"

int new_arena(struct arena** new)
{
	int error = 0;
	ENTER;
	
	void* ptr = NULL;
	
	if ((ptr = mmap(
		/* addr:   */  NULL,
		/* length: */  INITIAL_MMAP_LENGTH,
		/* prot:   */  PROT_READ | PROT_WRITE,
		/* flags:  */  MAP_ANONYMOUS | MAP_PRIVATE,
		/* fd:     */ -1,
		/* offset: */  0)) == MAP_FAILED)
	{
		fprintf(stderr, "%s: mmap(): %m\n", argv0);
		error = e_syscall_failed;
	}
	
	#ifdef DEBUGGING
	if (!error)
	{
		VALGRIND_MAKE_MEM_UNDEFINED(ptr, INITIAL_MMAP_LENGTH);
	}
	#endif
	
	struct arena* this = NULL;
	
	if (!error)
		error = smalloc((void**) &this, sizeof(*this));
	
	if (!error)
	{
		struct arena_header* header = ptr;
		struct arena_footer* footer = ptr + INITIAL_MMAP_LENGTH - sizeof(*footer);
		
		header->is_alloc = false;
		header->size = INITIAL_MMAP_LENGTH;
		
		dpv(header->size);
		
		header->prev = NULL;
		header->next = NULL;
		
		this->free_head = header;
		this->free_tail = header;
		
		footer->header = header;
		
		this->start = ptr;
		
		this->cap = INITIAL_MMAP_LENGTH;
		
		*new = this, ptr = NULL;
	}
	
	if (ptr && munmap(ptr, INITIAL_MMAP_LENGTH) < 0)
		fprintf(stderr, "%s: munmap(): %m\n", argv0),
		error = e_syntax_error;
	
	EXIT;
	return error;
}












