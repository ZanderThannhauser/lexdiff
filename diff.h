
enum edit_kind;
struct edits;
struct token_list;

unsigned diff(
	struct id_to_cost* idtoc,
	enum edit_kind* edits,
	struct token_list* master,
	struct token_list* compare);
