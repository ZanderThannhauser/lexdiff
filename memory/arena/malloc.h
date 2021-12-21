
struct arena;

int arena_malloc(
	struct arena* this,
	void** ptr, size_t size);
