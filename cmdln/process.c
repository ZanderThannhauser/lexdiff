
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include <debug.h>

#include <enums/error.h>

#include "usage_message.h"
#include "specification_path.h"
#include "master_path.h"
#include "compare_path.h"
#include "dotout_tokenizer.h"
#include "pretty_print.h"
#include "verbose.h"
#include "process.h"

void cmdln_process(int argc, char* const* argv)
{
	ENTER;
	
	int opt, option_index;
	const struct option long_options[] = {
		{"pretty-print",        no_argument, 0, 'p'},
		{"dotout",        required_argument, 0, 'd'},
		{"verbose",             no_argument, 0, 'v'},
		{"help",                no_argument, 0, 'h'},
		{ 0,                              0, 0,  0 },
	};
	
	while ((opt = getopt_long(argc, argv,
		"p" "d" "v" "h",
		long_options, &option_index)) >= 0)
	{
		switch (opt)
		{
			case 'p':
				should_pretty_print = true;
				break;
			
			case 'd':
				dotout_tokenizer = optarg;
				break;
			
			case 'v':
				verbose = true;
				break;
			
			case 'h':
				fputs(usage_message, stdout), exit(0);
				break;
			
			default:
			{
				fprintf(stderr, "zebu: unknown flag '%s'!\n", optarg);
				fputs(usage_message, stderr), exit(e_bad_cmdline_args);
				break;
			}
		}
	}
	
	if (false
		|| !(specification_path = argv[optind++])
		|| !(master_path = argv[optind++])
		|| !(compare_path = argv[optind++]))
	{
		fprintf(stderr, "zebu: missing arguments!\n");
		fputs(usage_message, stderr), exit(e_bad_cmdline_args);
	}
	
	dpvs(specification_path);
	dpvs(master_path);
	dpvs(compare_path);
	dpvs(dotout_tokenizer);
	dpvb(should_pretty_print);
	
	EXIT;
}






















