
struct diff_cell;
struct id_to_rule;
enum edit_kind;
struct token_list;

void pretty_print(
	struct diff_cell* table,
	struct token_list* btoks,
	struct token_list* atoks);
