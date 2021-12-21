
#include <debug.h>

#include <enums/error.h>

#include <defines/argv0.h>

#include "token.h"
#include "tokendata.h"

#include "read_char.h"
#include "read_token.h"
#include "read_statement.h"

#include "parse.h"

int parse(
	struct file_options** out_options,
	const char* pathname,
	struct arena* regex_arena)
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
			else if (ctd.string.len == 7 && !wmemcmp(ctd.string.data, L"unicode", 7))
				flags.is_unicode = true;
			else if (ctd.string.len == 6 && !wmemcmp(ctd.string.data, L"binary", 6))
				flags.is_unicode = false;
			else fprintf(stderr, "%s: no such option \"%.*ls\"!\n", argv0,
					(int) ctd.string.len, ctd.string.data),
				error = e_syntax_error;
			
			if (!error)
				error = read_token(fd, &cb, rb, &cc, &ct, &ctd);
			
			if (!error && ct == t_comma)
				error = read_token(fd, &cb, rb, &cc, &ct, &ctd);
		}
		
		// eat "t_semicolon":
		if (!error)
			error = read_token(fd, &cb, rb, &cc, &ct, &ctd);
	}
	
	while (!error && ct != t_EOF)
		error = read_statement(fd, &cb, rb, &cc, &ct, &ctd, regex_arena);
	
	TODO;
	
	if (fd > 0)
		close(fd);
	
	EXIT;
	return error;
}














