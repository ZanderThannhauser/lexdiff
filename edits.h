
struct edits
{
	enum {
		ek_insert,
		ek_update,
		ek_match,
		ek_delete,
	} kind;
	
	struct edits* prev;
};
