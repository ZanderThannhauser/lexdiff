
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

#include <id_to_rule/new.h>
#include <id_to_rule/free.h>

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

#include <mpq_print.h>

#include <diff_cell.h>

int main(int argc, char* const* argv)
{
	ENTER;
	
	cmdln_process(argc, argv);
	
	struct id_to_rule* idtor = new_id_to_rule();
	
	struct regex* tokenizer = parse_specification(idtor);
	
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
		
		struct token_list* btoks = tokenize(before_stream, tokenizer);
		struct token_list* atoks = tokenize(after_stream, tokenizer);
		
		struct diff_cell* table = diff(idtor, btoks, atoks);
		
		if (should_pretty_print)
		{
			pretty_print(table, idtor, btoks, atoks);
		}
		
		unsigned len = (btoks->n + 1) * (atoks->n + 1);
		
		fputs("score: ", stdout), mpq_print(table[len-1].total), putchar('\n');
		
		free_diff_table(table, len);
		
		free_token_list(btoks);
		free_token_list(atoks);
		
		fclose(before_stream);
		fclose(after_stream);
	}
	
	
	free_id_to_rule(idtor);
	
	free_regex(tokenizer);
	
	EXIT;
	return 0;
}















