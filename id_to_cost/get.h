
#include <gmp.h>

struct id_to_cost;

mpq_ptr id_to_cost_get_insert(
	struct id_to_cost* this,
	unsigned id);
	
mpq_ptr id_to_cost_get_update(
	struct id_to_cost* this,
	unsigned id);
	
mpq_ptr id_to_cost_get_match(
	struct id_to_cost* this,
	unsigned id);
	
mpq_ptr id_to_cost_get_delete(
	struct id_to_cost* this,
	unsigned id);
