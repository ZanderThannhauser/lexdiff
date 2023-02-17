
#include <gmp.h>

struct id_to_cost_node
{
	unsigned id;
	mpq_ptr insert, update, match, delete;
};

