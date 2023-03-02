
#include <gmp.h>

struct token
{
	struct token_rule* rule;
	
	char* chars;
	unsigned len;
	
	mpq_t value;
	
	unsigned refcount;
};

