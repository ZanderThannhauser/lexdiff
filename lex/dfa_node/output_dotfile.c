
#include <debug.h>

#include <enums/error.h>

#include <defines/argv0.h>

#include "struct.h"
#include "output_dotfile.h"

static int dfa_node_output_dotfile_slave(struct dfa_node* this, int outfd)
{
	int error = 0;
	size_t i, n;
	struct dfa_node_branch* ele;
	ENTER;
	
	dpv(this);
	
	if (!this->loop_guard)
	{
		this->loop_guard = true;
		
		if (this->rule)
		{
			dprintf(outfd, "\"%p\" [shape=doublecircle];\n", this);
		}
		else
		{
			dprintf(outfd, "\"%p\" [shape=circle];\n", this);
		}
		
		for (i = 0, n = this->transitions.n; i < n; i++)
		{
			ele = &(this->transitions.data[i]);
			
			char buffer[10];
			
			if (index("+-,."
				"0123456789"
				"abcdefghijklmnopqustuvwxyz", ele->value))
				buffer[0] = ele->value, buffer[1] = 0;
			else if (ele->value == '\n') strcpy(buffer, "\\\\n");
			else if (ele->value == '\r') strcpy(buffer, "\\\\r");
			else if (ele->value == '\t') strcpy(buffer, "\\\\t");
			else if (ele->value == '\\') strcpy(buffer, "\\\\");
			else snprintf(buffer, 10, "0x%X", ele->value);
			
			dprintf(outfd, "\t\"%p\" -> \"%p\" [label=\"%s\"]\n",
				this, ele->to, buffer);
			
			dfa_node_output_dotfile_slave(ele->to, outfd);
		}
		
		if (this->default_transition)
		{
			dprintf(outfd, "\t\"%p\" -> \"%p\" [label=\"default\"]\n",
				this, this->default_transition);
			
			dfa_node_output_dotfile_slave(this->default_transition, outfd);
		}
	}
	
	EXIT;
	return error;
}
	
int dfa_node_output_dotfile(
	struct dfa_node* this,
	const char* path)
{
	int error = 0;
	int fd = -1;
	ENTER;
	
	if ((fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, 0664)) < 0)
	{
		fprintf(stderr, "%s: open(\"%s\"): %m\n", argv0, path),
		error = e_syscall_failed;
	}
	
	if (!error)
	{
		dprintf(fd, "digraph G {\n");
		
		error = dfa_node_output_dotfile_slave(this, fd);
		
		dprintf(fd, "}\n");
	}
	
	if (fd > 0)
		close(fd);
	
	EXIT;
	return error;
}





















