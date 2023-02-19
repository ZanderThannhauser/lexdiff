
#include <debug.h>

#include "mpq_print.h"

void mpq_print(mpq_ptr mpq)
{
	ENTER;
	
	int sgn = mpq_sgn(mpq);
	
	if (!sgn)
		putchar('0');
	else
	{
		if (sgn < 0)
			putchar('-');
			
		mpq_t abs;
		
		mpq_init(abs);
		
		mpq_abs(abs, mpq);
		
		mpz_t whole;
		
		mpz_init(whole);
		
		mpz_fdiv_q(whole, mpq_numref(abs), mpq_denref(abs));
		
		if (mpz_sgn(whole))
		{
			gmp_printf("%Zu", whole);
		}
		
		mpz_clear(whole);
		
		mpz_t remainder;
		
		mpz_init(remainder);
		
		mpz_fdiv_r(remainder, mpq_numref(abs), mpq_denref(abs));
		
		if (mpz_sgn(remainder))
		{
			mpq_t fraction;
			
			mpq_init(fraction);
			
			mpz_set(mpq_numref(fraction), remainder);
			
			mpz_set(mpq_denref(fraction), mpq_denref(abs));
			
			mpq_canonicalize(fraction);
			
			{
				char* num = NULL;
				
				gmp_asprintf(&num, "%Zu", mpq_numref(fraction));
				
				static const char* lookup[] = {
					"⁰", "¹", "²", "³", "⁴", "⁵", "⁶", "⁷", "⁸", "⁹",
				};
				
				for (char* i = num; *i; i++)
					fputs(lookup[*i - '0'], stdout);
				
				free(num);
			}
			
			putchar('/');
			
			{
				char* den = NULL;
				
				gmp_asprintf(&den, "%Zu", mpq_denref(fraction));
				
				static const char* lookup[] = {
					"₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉"
				};
				
				for (char* i = den; *i; i++)
					fputs(lookup[*i - '0'], stdout);
				
				free(den);
			}
			
			mpq_clear(fraction);
		}
		
		mpz_clear(remainder);
		
		mpq_clear(abs);
	}
	
	EXIT;
}

