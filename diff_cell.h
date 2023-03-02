
#include <gmp.h>

#include "edit_kind.h"

struct diff_cell
{
	enum edit_kind action;
	mpq_t total, delta;
	
	unsigned within_index;
};
