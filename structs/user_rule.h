
struct user_rule
{
	unsigned id;
	
	bool match_content;
	
	bool is_deduction; // otherwise: extra credit
	double weight;
	bool is_percentage;
	
	struct {
		double addition;
		double modification;
		double identification; // cost of no change
		double deletion;
	} cost;
};

