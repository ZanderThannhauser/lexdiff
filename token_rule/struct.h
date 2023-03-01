
#include <stdbool.h>
#include <gmp.h>

struct token_rule
{
	mpq_t insert;
	mpq_t update;
	mpq_t match;
	mpq_t delete;
	
	struct {
		struct within {
			mpq_t tolerance;
			bool is_percentage;
			mpq_t points;
		}* data;
		unsigned n, cap;
	} withins;
	
	unsigned refcount;
};

