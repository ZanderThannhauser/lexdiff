
#include <debug.h>

#include <enums/error.h>

#include <memory/arena/new.h>
#include <memory/arena/free.h>

#include <cmdln/flags.h>
#include <cmdln/process.h>
#include <cmdln/free.h>

#include <parser/parse.h>

#include <parser/file_options/free.h>

#if DEBUGGING
int debug_depth;
#endif

int main(int argc, char* const* argv)
{
	int error = 0;
	struct cmdln_flags* flags = NULL;
	struct arena* regex_arena = NULL;
	struct file_options* options = NULL;
	ENTER;
	
	error = 0
		// process cmdln flags
		?: process_flags(&flags, argc, argv)
		// setup memory arena for nfa & dfa structures:
		?: new_arena(&regex_arena)
		// parse patternfile
		?: parse(&options, flags->patternfile, regex_arena)
		;
	
	TODO;
	
	// for each statement:
		// convert to nfa
	TODO;
	
	// union all nfas together
	TODO;
	
	// convert to dfa
	TODO;
	
	// tokenize each file
		// remember text, line of each token
	TODO;
	
	// allocate 2D array of grades
	TODO;
	
	// fill out with grades
	TODO;
	
	// start at bottom right, moving to top left
		// storing operations as you go
	TODO;
	
	// print out in HTML? or terminal escape codes (diff-style)?
	TODO;
	
	free_file_options(options);
	
	free_arena(regex_arena);
	
	free_flags(flags);
	
	EXIT;
	
	if (error == e_show_usage)
		error = 0;
	
	return error;
}









