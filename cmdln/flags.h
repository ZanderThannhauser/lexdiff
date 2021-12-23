
#include <stdbool.h>

struct cmdln_flags
{
	char* patternfile;
	
	bool verbose;
	
	bool output_dotfile;
	
	char* solution_textfile;
	char* student_textfile;
};

