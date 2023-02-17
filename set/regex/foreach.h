
struct regexset;
struct regex;

void regexset_foreach(
	struct regexset* this,
	void (*callback)(struct regex*));
