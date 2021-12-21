
#ifdef DEBUGGING

#include <debug.h>

#include "struct.h"
#include "header.h"
#include "footer.h"
#include "prettyprint.h"

void arena_prettyprint(
	struct arena* this)
{
	void *i, *n;
	struct arena_header* header;
	struct arena_footer* footer;
	ENTER;
	
	dpv(this->free_head);
	dpv(this->free_tail);
	
	for (i = this->start, n = i + this->cap; i < n; i += header->size)
	{
		header = i;
		dprintf(
			"@%p: is_alloc = %s, size = %4lu\n",
			header,
			header->is_alloc ? " true" : "false",
			header->size);
		
		footer = i + header->size - sizeof(*footer);
/*		dprintf(*/
/*			"@%p: header = %p\n",*/
/*			footer,*/
/*			footer->header);*/
/*		*/
		assert(footer->header == header);
	}
	
	EXIT;
}

#endif
