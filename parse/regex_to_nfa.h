
struct zebu_regex;

struct nfa {
	struct regex *start, *accepts;
} regex_to_nfa(struct zebu_regex* regex);
