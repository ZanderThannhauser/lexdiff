
struct arena;

int arena_sbrk(
	struct arena* this,
	void** out,
	size_t size);
