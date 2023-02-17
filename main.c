
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <debug.h>

#include <memory/smalloc.h>

#include <cmdln/process.h>
#include <cmdln/dotout_tokenizer.h>
#include <cmdln/master_path.h>
#include <cmdln/compare_path.h>
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
			TODO;
			exit(1);
		}
		
		regex_dotout(tokenizer, stream);
		
		fclose(stream);
	}
	else
	{
		FILE *mstream, *cstream;
		
		if (!(mstream = fopen(master_path, "r")))
		{
			TODO;
			exit(1);
		}
		
		if (!(cstream = fopen(compare_path, "r")))
		{
			TODO;
			exit(1);
		}
		
		struct token_list* mtokens = tokenize(mstream, tokenizer);
		struct token_list* ctokens = tokenize(cstream, tokenizer);
		
		enum edit_kind* edits = smalloc(sizeof(*edits) * (mtokens->n + ctokens->n));
		
		unsigned number_of_edits = diff(idtoc, edits, mtokens, ctokens);
		
		dpv(number_of_edits);
		
		if (should_pretty_print)
		{
			pretty_print(mtokens, ctokens, edits, number_of_edits);
		}
		
		free_token_list(mtokens);
		free_token_list(ctokens);
		
		fclose(mstream);
		fclose(cstream);
		
		free(edits);
	}
	
	free_id_to_cost(idtoc);
	
	free_regex(tokenizer);
	
	EXIT;
	return 0;
}















