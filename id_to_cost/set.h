
#include <gmp.h>

struct id_to_cost;

void id_to_cost_set(
	struct id_to_cost* this,
	unsigned id,
	mpq_ptr insert,
	mpq_ptr match,
	mpq_ptr update,
	mpq_ptr delete);
