
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include <debug.h>

#include <enums/error.h>

#include <defines/argv0.h>

#include "usage_message.h"
#include "specification_path.h"
#include "before_path.h"
#include "width.h"
#include "after_path.h"
#include "dotout_tokenizer.h"
#include "pretty_print.h"
#include "verbose.h"
#include "process.h"

void cmdln_process(int argc, char* const* argv)
{
	ENTER;
	
	int opt, option_index;
	const struct option long_options[] = {
		{"width",         required_argument, 0, 'W'},
		{"pretty-print",        no_argument, 0, 'p'},
		{"dotout",        required_argument, 0, 'd'},
		{"verbose",             no_argument, 0, 'v'},
		{"help",                no_argument, 0, 'h'},
		{ 0,                              0, 0,  0 },
	};
	
	while ((opt = getopt_long(argc, argv,
		"W:" "p" "d:" "v" "h",
		long_options, &option_index)) >= 0)
	{
		switch (opt)
		{
			case 'W':
				width = atoi(optarg);
				break;
			
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
	
	if (width <= 0)
		width = 60;
	
	specification_path = argv[optind++];
	before_path = argv[optind++];
	after_path = argv[optind++];
	
	if (!(specification_path && (dotout_tokenizer || (before_path && after_path))))
	{
		fprintf(stderr, "%s: missing arguments!\n", argv0);
		fputs(usage_message, stderr), exit(e_bad_cmdline_args);
	}
	
	dpvs(specification_path);
	dpvs(before_path);
	dpvs(after_path);
	dpvs(dotout_tokenizer);
	dpvb(should_pretty_print);
	
	EXIT;
}






















