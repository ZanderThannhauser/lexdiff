
enum edit_kind;
struct token_list;

struct diff_cell* diff(
	struct token_list* before,
	struct token_list* after);
	
void free_diff_table(
	struct diff_cell* table,
	unsigned len);
