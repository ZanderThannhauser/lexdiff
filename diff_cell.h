
#include <gmp.h>

struct diff_cell
{
	enum edit_kind action;
	mpq_t total, delta;
	
	unsigned within_index;
};
