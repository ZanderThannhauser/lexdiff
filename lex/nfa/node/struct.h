
struct nfa_node
{
	struct {
		struct nfa_node_branch {
			wchar_t codepoint;
			struct nfa_node* node;
		}* data;
		size_t n, cap;
	} normal_branches;
	
	struct {
		struct nfa_node** data;
		size_t n, cap;
	} default_branches;
	
	struct {
		struct nfa_node** data;
		size_t n, cap;
	} lambda_branches;
};

