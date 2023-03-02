
#include <string.h>

#include <debug.h>

#include <memory/smalloc.h>

#include <token_rule/inc.h>

#include "struct.h"
#include "new.h"

struct token* new_token(
	struct token_rule* rule,
	const char* chars,
	unsigned len)
{
	ENTER;
	
	struct token *this = smalloc(sizeof(*this));
	
	this->rule = inc_token_rule(rule);
	
	this->chars = strndup(chars, len);
	this->len = len;
	
	mpq_init(this->value);
	
	this->refcount = 1;
	
	EXIT;
	return this;
}

