
struct diff_cell;
struct id_to_rule;
enum edit_kind;
struct token_list;

void escape(char* moving, unsigned len);

void pretty_print(
	struct diff_cell* table,
	struct id_to_rule* idtor,
	struct token_list* btoks,
	struct token_list* atoks);
