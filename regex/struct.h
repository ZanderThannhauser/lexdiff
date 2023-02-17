
struct regex
{
	struct regex* transitions[256];
	
	struct {
		struct regex** data;
		unsigned n, cap;
	} lambdas;
	
	// 0 means nonaccepting, 1 means whitespace, otherwise: token id.
	unsigned accepts;
};

