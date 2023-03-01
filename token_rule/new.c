
#include <debug.h>

#include <memory/smalloc.h>

#include "struct.h"
#include "new.h"

struct token_rule* new_token_rule()
{
	ENTER;
	
	struct token_rule* this = smalloc(sizeof(*this));
	
	mpq_init(this->insert), mpq_set_ui(this->insert, 0, 1);
	mpq_init(this->match),  mpq_set_ui(this->match,  0, 1);
	mpq_init(this->update), mpq_set_ui(this->update, 0, 1);
	mpq_init(this->delete), mpq_set_ui(this->delete, 0, 1);
	
	this->withins.data = NULL;
	this->withins.n = 0;
	this->withins.cap = 0;
	
	this->refcount = 1;
	
	EXIT;
	return this;
}

