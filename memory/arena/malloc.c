
#include <debug.h>

/*#include "private/find_block.h"*/

#include "header.h"
#include "footer.h"

#include "struct.h"
#include "prettyprint.h"
#include "sbrk.h"
#include "malloc.h"

static int private_arena_find_block(
	struct arena* this,
	void** out, size_t* size)
{
	int error = 0;
	void* ptr;
	struct arena_header *i, *block = NULL;
	ENTER;
	
	dpv(*size);
	
	for (i = this->free_head; !block && i; i = i->next)
		if (!i->is_alloc && *size <= i->size)
			ptr = block = i;
	
	if (!block)
	{
		error = arena_sbrk(this, &ptr, *size);
	}
	else if (block->size - *size >= sizeof(struct arena_header) + sizeof(struct arena_footer))
	{
		size_t split_size = block->size - *size;
		dpv(split_size);
		
		struct arena_header* newblock = ptr + *size;
		struct arena_footer* footer = ptr + block->size - sizeof(*footer);
		
		dpv(newblock);
		
		// do split, adjust linked-list:
		newblock->is_alloc = false;
		newblock->size = split_size;
		
		newblock->prev = block->prev;
		newblock->next = block->next;
		
		footer->header = newblock;
		
		if (block->prev)
			block->prev->next = newblock;
		else
			this->free_head = newblock;
		
		if (block->next)
			block->next->prev = newblock;
		else
			this->free_tail = newblock;
	}
	else
	{
		// don't do split, remove from linked-list
		TODO;
	}
	
	if (!error)
		*out = ptr;
	
	EXIT;
	return error;
}


int arena_malloc(
	struct arena* this,
	void** out, size_t user_size)
{
	int error = 0;
	void* ptr;
	size_t block_size;
	struct arena_header* header;
	struct arena_footer* footer;
	ENTER;
	
	block_size = 0
		+ sizeof(*header)
		+ user_size
		+ sizeof(*footer);
	
	error = private_arena_find_block(this, &ptr, &block_size);
	
	if (!error)
	{
		dpv(ptr);
		
		header = ptr;
		footer = ptr + block_size - sizeof(*footer);
		
		header->is_alloc = true;
		header->size = block_size;
		
		footer->header = header;
		
		void* payload = ptr + sizeof(*header);
		
		VALGRIND_MAKE_MEM_UNDEFINED(payload, user_size);
		
		*out = payload;
	}
	
	arena_prettyprint(this);
	
	EXIT;
	return error;
}











