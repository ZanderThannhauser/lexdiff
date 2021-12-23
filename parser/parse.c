
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#include <debug.h>

#include <enums/error.h>

#include <defines/argv0.h>

#include <memory/arena/new.h>
#include <memory/arena/free.h>

#include <lex/convert/convert.h>
#include <lex/nfa_node/new.h>
#include <lex/nfa_node/add_lambda_branch.h>

#include "token.h"
#include "token_data.h"

#include "read_char.h"
#include "read_token.h"
#include "read_rule.h"

#include "parse.h"

int parse(
	struct file_options** out_options,
	const char* pathname,
	struct memory_arena* dfa_arena,
	struct dfa_node** out_start)
{
	int error = 0;
	ENTER;
	
	dpvs(pathname);
	
	uint8_t buffer[4096], *start = 0, *end = 0;
	
	int read_byte(int fd, uint8_t* cb)
	#define rb (read_byte)
	{
		int error = 0;
		ssize_t retval;
		
		if (start < end)
			*cb = *start++;
		else if ((retval = read(fd, buffer, sizeof(buffer))) < 0)
			fprintf(stderr, "%s: read(): %m\n", argv0),
			error = e_syscall_failed;
		else if (!retval)
			*cb = '\0'; // EOF
		else
		{
			start = buffer,
			end = start + retval,
			*cb = *start++;
		}
		
		return error;
	}
	
	int fd = -1;
	uint8_t cb;
	wchar_t cc;
	enum token ct;
	union tokendata ctd;
	
	if ((fd = open(pathname, O_RDONLY)) < 0)
		fprintf(stderr, "%s: open(\"%s\"): %m\n", argv0, pathname),
		error = e_syscall_failed;
	else error = 0
		?: read_byte(fd, &cb)
		?: read_char(fd, &cb, rb, &cc)
		?: read_token(fd, &cb, rb, &cc, &ct, &ctd);
	
	struct {
		bool is_unicode;
	} flags = {
		.is_unicode = true
	};
	
	while (!error && ct == t_options)
	{
		// eat "t_options":
		error = read_token(fd, &cb, rb, &cc, &ct, &ctd);
		
		
		if (!error && ct != t_colon)
			error = e_syntax_error;
		
		// eat "t_colon":
		if (!error)
			error = read_token(fd, &cb, rb, &cc, &ct, &ctd);
		
		while (!error && ct != t_semicolon)
		{
			if (ct != t_string)
				error = e_syntax_error;
			else
			{
				if (ctd.string.len == 7 && !wmemcmp(ctd.string.data, L"unicode", 7))
					flags.is_unicode = true;
				else if (ctd.string.len == 6 && !wmemcmp(ctd.string.data, L"binary", 6))
					flags.is_unicode = false;
				else
				{
					fprintf(stderr, "%s: no such option \"%.*ls\"!\n", argv0,
						(int) ctd.string.len, ctd.string.data),
					error = e_syntax_error;
				}
				
				free(ctd.string.data);
			}
			
			if (!error)
				error = read_token(fd, &cb, rb, &cc, &ct, &ctd);
			
			if (!error && ct == t_comma)
				error = read_token(fd, &cb, rb, &cc, &ct, &ctd);
		}
		
		// eat "t_semicolon":
		if (!error)
			error = read_token(fd, &cb, rb, &cc, &ct, &ctd);
	}
	
	if (!error && ct == t_EOF)
	{
		TODO; // "empty file!"
		error = 1;
	}
	
	struct memory_arena* nfa_arena = NULL;
	
	struct nfa_node* start_nfa;
	struct nfa_node* start_rule;
	
	if (!error)
		error = 0
			?: new_memory_arena(&nfa_arena)
			?: new_nfa_node(&start_nfa, nfa_arena);
	
	unsigned rule_id = 0;
	while (!error && ct != t_EOF)
	{
		error = 0
			?: read_rule(fd, &cb, rb, &cc, &ct, &ctd, nfa_arena, rule_id,
				&start_rule)
			?: nfa_node_add_lambda_branch(start_nfa, nfa_arena, start_rule);
		
		rule_id++;
	}
	
	if (!error)
		error = convert_nfa_to_dfa(start_nfa, nfa_arena, dfa_arena, out_start);
	
	if (fd > 0)
		close(fd);
	
	free_memory_arena(nfa_arena);
	
	EXIT;
	return error;
}














