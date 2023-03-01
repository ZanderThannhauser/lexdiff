
#include <stdlib.h>

#include <debug.h>

#include "struct.h"
#include "add_within.h"

void token_rule_add_within(
	struct token_rule* this,
	mpq_t tolerance,
	bool is_percentage,
	mpq_t points)
{
	ENTER;
	
	#ifdef DEBUGGING
	gmp_printf("tolerance == %Qi\n", tolerance);
	gmp_printf("points == %Qi\n", points);
	#endif
	
	if (this->withins.n == this->withins.cap)
	{
		this->withins.cap = this->withins.cap << 1 ?: 1;
		this->withins.data = realloc(this->withins.data,
			sizeof(*this->withins.data) * this->withins.cap);
	}
	
	struct within* within = &this->withins.data[this->withins.n++];
	
	mpq_init(within->tolerance), mpq_set(within->tolerance, tolerance);
	within->is_percentage = is_percentage;
	mpq_init(within->points), mpq_set(within->points, points);
	
	EXIT;
}

