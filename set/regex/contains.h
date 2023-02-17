
#include <stdbool.h>

struct regexset;
struct regex;

bool regexset_contains(
	const struct regexset* this,
	struct regex* ele);
