
struct regex
{
	struct regex* transitions[256];
	
	struct {
		struct regex** data;
		unsigned n, cap;
	} lambdas;
	
	struct token_rule* accepts; // NULL if not accepting
};

