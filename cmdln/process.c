#include <getopt.h>

#include <debug.h>

#include <enums/error.h>

#include <memory/smalloc.h>

#include "flags.h"
#include "usage.h"
#include "process.h"

int process_flags(
	struct cmdln_flags** outgoing,
	int argc, char* const* argv)
{
	int error = 0;
	ENTER;
	
	bool verbose = false;
	bool output_dotfile = false;
	
	char* patternfile = "patternfile";
	char* solution_textfile = NULL;
	char* student_textfile = NULL;
	
	int option_index = 0;
	static struct option long_options[] = {
		{"help",        no_argument,       0, 'h'},
		{"patternfile", required_argument, 0, 'f'},
		{"verbose",     no_argument,       0, 'v'},
		{"dotfile",     no_argument,       0, 'd'},
		{ 0,            0,                 0,  0 }};
	
	int opt;
	while (!error && (opt = getopt_long(argc, argv, "hf:vd", long_options, &option_index)) > 0)
	{
		switch (opt ?: option_index)
		{
			case 'f':
				patternfile = optarg;
				break;
			
			case 'h':
				error = e_show_usage;
				break;
			
			case 'v':
				verbose = true;
				break;
			
			case 'd':
				output_dotfile = true;
				break;
			
			default:
				error = e_bad_cmdln_args;
				break;
		}
	}
	
	if (!error && !output_dotfile && (false
		|| !(solution_textfile = argv[optind++])
		|| !(student_textfile = argv[optind++])))
	{
		error = e_bad_cmdln_args;
	}
	
	if (error)
		usage();
	
	struct cmdln_flags* flags = NULL;
	
	if (!error)
		error = smalloc((void**) &flags, sizeof(*flags));
	
	if (!error)
	{
		dpvs(patternfile);
		dpvs(solution_textfile);
		dpvs(student_textfile);
		
		flags->patternfile = patternfile;
		flags->verbose = verbose;
		flags->output_dotfile = output_dotfile;
		
		flags->solution_textfile = solution_textfile;
		flags->student_textfile = student_textfile;
		
		*outgoing = flags;
	}
	
	EXIT;
	return error;
}










