
#include <gmp.h>

struct zebu_expression;
struct zebu_expression_highest;

mpq_ptr parse_expression_highest(
	struct zebu_expression_highest* expression);

mpq_ptr parse_expression(
	struct zebu_expression* expression);
