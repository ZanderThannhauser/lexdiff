
#include <gmp.h>

#include "init_set.h"

void mpq_init_set(mpq_ptr ptr, mpq_ptr src)
{
	mpq_init(ptr);
	mpq_set(ptr, src);
}
