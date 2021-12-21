
struct arena;

int arena_realloc(
	struct arena* this,
	void** ptr, size_t size);
