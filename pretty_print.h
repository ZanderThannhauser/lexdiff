
struct id_to_cost;
enum edit_kind;
struct token_list;

void pretty_print(
	struct id_to_cost* idtoc,
	struct token_list* before,
	struct token_list* after,
	enum edit_kind* edits,
	unsigned number_of_edits);
