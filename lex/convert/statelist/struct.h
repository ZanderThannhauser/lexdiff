
#include <stdlib.h>

struct nfa_node;

struct statelist
{
	struct nfa_node** states;
	size_t n, cap;
};

