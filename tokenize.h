
#include <stdio.h>

struct regex;
struct id_to_rule;

struct token_list* tokenize(
	FILE* stream,
	struct regex* tokenizer);

