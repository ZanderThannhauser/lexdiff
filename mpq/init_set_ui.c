
#include <gmp.h>

#include "init_set_ui.h"

void mpq_init_set_ui(mpq_ptr ptr, unsigned a, unsigned b)
{
	mpq_init(ptr);
	mpq_set_ui(ptr, a, b);
}
