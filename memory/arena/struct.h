
struct arena
{
	void *start;
	
	void *free_head, *free_tail;
	
	size_t cap;
};

