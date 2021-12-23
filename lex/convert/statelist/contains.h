
#include <stdbool.h>

struct nfa_node;
struct statelist;

bool statelist_contains(struct statelist* this, struct nfa_node* nfa_node);
