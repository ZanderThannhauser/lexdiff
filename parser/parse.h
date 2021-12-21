
struct file_options;

int parse(
	struct file_options** out_options,
	const char* pathname,
	struct arena* regex_arena);
