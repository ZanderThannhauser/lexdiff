
struct dfa_node
{
	struct {
		struct dfa_node_branch {
			wchar_t value;
			struct dfa_node* to;
		}* data;
		size_t n, cap;
	} transitions;
	
	struct dfa_node* default_transition;
	
	bool loop_guard;
	
	// used to indicate information about this rule (weight)
	// also used to indicate if this is a final state:
	struct user_rule* rule;
};

