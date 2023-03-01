
enum edit_kind;
struct token_list;

struct diff_cell* diff(
	struct id_to_rule* idtoc,
	struct token_list* master,
	struct token_list* compare);
	
void free_diff_table(
	struct diff_cell* table,
	unsigned len);
