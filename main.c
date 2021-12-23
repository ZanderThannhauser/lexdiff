
#include <stdio.h>

#include <debug.h>

#include <enums/error.h>

#include <memory/arena/new.h>
#include <memory/arena/free.h>

#include <cmdln/flags.h>
#include <cmdln/process.h>
#include <cmdln/free.h>

#include <lex/dfa_node/output_dotfile.h>

#include <parser/parse.h>
#include <parser/file_options/free.h>

#if DEBUGGING
int debug_depth;
#endif

int main(int argc, char* const* argv)
{
	int error = 0;
	struct cmdln_flags* flags = NULL;
	struct memory_arena* dfa_arena = NULL;
	struct file_options* options = NULL;
	struct dfa_node* state_machine = NULL;
	ENTER;
	
	error = 0
		// process cmdln flags
		?: process_flags(&flags, argc, argv)
		// setup memory arena for nfa & dfa structures:
		?: new_memory_arena(&dfa_arena)
		// parse patternfile
		?: parse(&options, flags->patternfile, dfa_arena, &state_machine);
	
	if (!error)
	{
		if (flags->output_dotfile)
		{
			// ouput dotfile
			error = dfa_node_output_dotfile(state_machine, "output.gv");
		}
		else
		{
			// tokenize each file
			// this will produce an array of pointers to `struct user_token`
			TODO;
			
			// calculate costs (percentages, insert, delete, modify)
			// for user_rules in the *first* list
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
		}
	}
	
	free_file_options(options);
	
	free_memory_arena(dfa_arena);
	
	free_flags(flags);
	
	if (error == e_show_usage)
		error = 0;
	
	EXIT;
	verprintf("exit(%i);\n", error);
	return error;
}









