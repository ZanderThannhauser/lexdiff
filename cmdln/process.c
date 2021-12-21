
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
	
	char* patternfile = "patternfile";
	char* solution_textfile = NULL;
	char* student_textfile = NULL;
	
	int opt;
	while (!error && (opt = getopt(argc, argv, "hf:")) > 0)
	{
		switch (opt)
		{
			case 'f':
				patternfile = optarg;
				break;
			
			case 'h':
				error = e_show_usage;
				break;
			
			default:
				error = e_bad_cmdln_args;
				break;
		}
	}
	
	if (!error && (false
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
		flags->solution_textfile = solution_textfile;
		flags->student_textfile = student_textfile;
		
		*outgoing = flags;
	}
	
	EXIT;
	return error;
}










