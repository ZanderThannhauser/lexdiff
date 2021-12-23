
struct memory_arena;
struct arena;
struct dfa_node;
struct file_options;

int parse(
	struct file_options** out_options,
	const char* pathname,
	struct memory_arena* regex_arena,
	struct dfa_node** out_node);
