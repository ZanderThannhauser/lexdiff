
struct user_rule;

struct nfa_node
{
	struct {
		struct nfa_node_branch {
			wchar_t value;
			struct nfa_node* to;
		}* data;
		size_t n, cap;
	} transitions;
	
	struct nfa_node* default_transition;
	
	struct {
		struct nfa_node** data;
		size_t n, cap;
	} lambda_transitions;
	
	// used to indicate information about this rule (weight)
	// also used to indicate if this is a final state:
	struct user_rule* rule;
};

