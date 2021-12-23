
struct state_list;
struct dfa_node;

struct bundle
{
	struct statelist* statelist; // must be first
	struct dfa_node* combined;
};
