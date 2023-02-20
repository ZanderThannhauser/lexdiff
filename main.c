
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <debug.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include <memory/smalloc.h>

#include <cmdln/process.h>
#include <cmdln/dotout_tokenizer.h>
#include <cmdln/before_path.h>
#include <cmdln/after_path.h>
#include <cmdln/pretty_print.h>

#include <id_to_cost/new.h>
#include <id_to_cost/free.h>

#include <parse/parse.h>

#include <regex/dotout.h>
#include <regex/free.h>

#include <token.h>
#include <tokenize.h>

#include <edit_kind.h>

#include <diff.h>

#include <token_list/struct.h>
#include <token_list/free.h>

#include <pretty_print.h>

int main(int argc, char* const* argv)
{
	ENTER;
	
	cmdln_process(argc, argv);
	
	struct id_to_cost* idtoc = new_id_to_cost();
	
	struct regex* tokenizer = parse_specification(idtoc);
	
	if (dotout_tokenizer)
	{
		FILE* stream = fopen(dotout_tokenizer, "w");
		
		if (!stream)
		{
			fprintf(stderr, "%s: fopen(\"%s\"): %m\n", argv0, dotout_tokenizer);
			exit(e_syscall_failed);
		}
		
		regex_dotout(tokenizer, stream);
		
		fclose(stream);
	}
	else
	{
		FILE *before_stream, *after_stream;
		
		if (!(before_stream = fopen(before_path, "r")))
		{
			fprintf(stderr, "%s: fopen(\"%s\"): %m\n", argv0, before_path);
			exit(e_syscall_failed);
		}
		
		if (!(after_stream = fopen(after_path, "r")))
		{
			fprintf(stderr, "%s: fopen(\"%s\"): %m\n", argv0, after_path);
			exit(e_syscall_failed);
		}
		
		struct token_list* before_tokens = tokenize(before_stream, tokenizer);
		struct token_list* after_tokens = tokenize(after_stream, tokenizer);
		
		enum edit_kind* edits = smalloc(sizeof(*edits) * (before_tokens->n + after_tokens->n));
		
		unsigned number_of_edits = diff(idtoc, edits, before_tokens, after_tokens);
		
		dpv(number_of_edits);
		
		if (should_pretty_print)
		{
			pretty_print(idtoc, before_tokens, after_tokens, edits, number_of_edits);
		}
		
		free_token_list(before_tokens);
		free_token_list(after_tokens);
		
		fclose(before_stream);
		fclose(after_stream);
		
		free(edits);
	}
	
	free_id_to_cost(idtoc);
	
	free_regex(tokenizer);
	
	EXIT;
	return 0;
}















