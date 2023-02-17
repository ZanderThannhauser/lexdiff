
enum edit_kind;
struct token_list;

void pretty_print(
	struct token_list* mlist,
	struct token_list* clist,
	enum edit_kind* edits,
	unsigned number_of_edits);
