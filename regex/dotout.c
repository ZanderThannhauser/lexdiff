
#include <string.h>

#include <debug.h>

#include <quack/new.h>
#include <quack/append.h>
#include <quack/is_nonempty.h>
#include <quack/pop.h>
#include <quack/free.h>

#include <set/regex/new.h>
#include <set/regex/add.h>
#include <set/regex/free.h>

#include <regex/struct.h>

#include "dotout.h"

void regex_dotout(
	struct regex* start,
	FILE* stream)
{
	ENTER;
	
	fprintf(stream, "digraph {" "\n");
	
	fprintf(stream, "rankdir = LR" "\n");
	
	struct regexset* queued = new_regexset();
	
	struct quack* todo = new_quack();
	
	regexset_add(queued, start);
	
	quack_append(todo, start);
	
	while (quack_is_nonempty(todo))
	{
		struct regex* regex = quack_pop(todo);
		
		if (regex->accepts)
		{
			fprintf(stream, ""
				"\"%p\" [" "\n"
					"shape = doublecircle" "\n"
					"label = \"%u\"" "\n"
				"];" "\n"
			"", regex, regex->accepts);
		}
		else
		{
			fprintf(stream, ""
				"\"%p\" [" "\n"
					"shape = circle" "\n"
					"label = \"\"" "\n"
				"];" "\n"
			"", regex);
		}
		
		for (unsigned i = 0; i < 256; i++)
		{
			struct regex* to = regex->transitions[i];
			
			if (to)
			{
				char label[300];
				
				switch (i)
				{
					case '\n':
						strcpy(label, "'\\\\n'");
						break;
					
					case '\t':
						strcpy(label, "'\\\\t'");
						break;
					
					case '\"':
						strcpy(label, "'\\\\\\\\\\\"'");
						break;
					
					case '\\':
						strcpy(label, "'\\\\'");
						break;
					
					case ' ':
					case '/':
					case '!':
					case '@':
					case '$':
					case '^':
					case '&':
					case '*':
					case '(':
					case ')':
					case '-':
					case '_':
					case '+':
					case '=':
					case '~':
					case '`':
					case '|':
					case '[': case ']':
					case '{': case '}':
					case ':': case ';':
					case '<': case '>':
					case ',': case '.':
					case '0' ... '9':
					case 'a' ... 'z':
					case 'A' ... 'Z':
						sprintf(label, "\'%c\'", i);
						break;
					
					default:
						sprintf(label, "0x%02hhX", i);
						break;
				}
				
				dpvs(label);
				
				fprintf(stream, "\"%p\" -> \"%p\" [label = \"%s\"];" "\n", regex, to, label);
				
				if (regexset_add(queued, to))
					quack_append(todo, to);
			}
		}
	}
	
	fprintf(stream, "}" "\n");
	
	free_regexset(queued);
	
	free_quack(todo);
	
	EXIT;
}













