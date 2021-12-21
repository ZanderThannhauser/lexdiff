
#include <stddef.h>
#include <stdbool.h>

struct arena_header
{
	bool is_alloc;
	size_t size;
	struct arena_header *prev, *next;
};

