
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>

struct zebu_token
{
	unsigned char* data;
	unsigned len, refcount, line;
};

struct zebu_$start
{
	struct zebu_root* root;
	unsigned refcount, startline, endline;
};

struct zebu_charset
{
	struct zebu_charset_union* inner;
	unsigned refcount, startline, endline;
};

struct zebu_charset_high
{
	struct zebu_token* character;
	struct zebu_charset* inner;
	unsigned refcount, startline, endline;
};

struct zebu_charset_inter
{
	struct zebu_charset_range* inner;
	struct zebu_charset_inter* sub;
	unsigned refcount, startline, endline;
};

struct zebu_charset_prefix
{
	struct zebu_token* emark;
	struct zebu_charset_high* inner;
	unsigned refcount, startline, endline;
};

struct zebu_charset_range
{
	struct zebu_charset_prefix* left;
	struct zebu_charset_prefix* right;
	unsigned refcount, startline, endline;
};

struct zebu_charset_union
{
	struct zebu_charset_inter* inner;
	struct zebu_charset_union* sub;
	unsigned refcount, startline, endline;
};

struct zebu_expression
{
	struct zebu_expression_additive* inner;
	unsigned refcount, startline, endline;
};

struct zebu_expression_additive
{
	struct zebu_token* add;
	struct zebu_expression_multiplicative* inner;
	struct zebu_expression_additive* left;
	struct zebu_expression_multiplicative* right;
	struct zebu_token* sub;
	unsigned refcount, startline, endline;
};

struct zebu_expression_highest
{
	struct zebu_expression* inner;
	struct zebu_token* literal;
	unsigned refcount, startline, endline;
};

struct zebu_expression_multiplicative
{
	struct zebu_token* div;
	struct zebu_expression_prefix* inner;
	struct zebu_expression_multiplicative* left;
	struct zebu_token* mul;
	struct zebu_expression_prefix* right;
	unsigned refcount, startline, endline;
};

struct zebu_expression_prefix
{
	struct zebu_expression_highest* inner;
	struct zebu_token* negate;
	unsigned refcount, startline, endline;
};

struct zebu_regex
{
	struct zebu_regex_union* inner;
	unsigned refcount, startline, endline;
};

struct zebu_regex_concat
{
	struct zebu_regex_suffix* inner;
	struct zebu_regex_concat* sub;
	unsigned refcount, startline, endline;
};

struct zebu_regex_high
{
	struct zebu_token* character;
	struct zebu_charset* charset;
	struct zebu_token* string;
	struct zebu_regex* sub;
	unsigned refcount, startline, endline;
};

struct zebu_regex_suffix
{
	struct zebu_token* asterisk;
	struct zebu_regex_high* inner;
	struct zebu_token* plus;
	struct zebu_token* qmark;
	unsigned refcount, startline, endline;
};

struct zebu_regex_union
{
	struct zebu_regex_concat* inner;
	struct zebu_regex_union* sub;
	unsigned refcount, startline, endline;
};

struct zebu_root
{
	struct {
		struct zebu_token_rule** data;
		unsigned n, cap;
	} rules;
	unsigned refcount, startline, endline;
};

struct zebu_token_rule
{
	struct zebu_expression* delete;
	struct zebu_expression* insert;
	struct zebu_expression* match;
	struct zebu_regex* regex;
	struct zebu_expression* update;
	unsigned refcount, startline, endline;
};



extern struct zebu_token* inc_zebu_token(struct zebu_token* token);
extern struct zebu_$start* inc_zebu_$start(struct zebu_$start* ptree);
extern struct zebu_charset* inc_zebu_charset(struct zebu_charset* ptree);
extern struct zebu_charset_high* inc_zebu_charset_high(struct zebu_charset_high* ptree);
extern struct zebu_charset_inter* inc_zebu_charset_inter(struct zebu_charset_inter* ptree);
extern struct zebu_charset_prefix* inc_zebu_charset_prefix(struct zebu_charset_prefix* ptree);
extern struct zebu_charset_range* inc_zebu_charset_range(struct zebu_charset_range* ptree);
extern struct zebu_charset_union* inc_zebu_charset_union(struct zebu_charset_union* ptree);
extern struct zebu_expression* inc_zebu_expression(struct zebu_expression* ptree);
extern struct zebu_expression_additive* inc_zebu_expression_additive(struct zebu_expression_additive* ptree);
extern struct zebu_expression_highest* inc_zebu_expression_highest(struct zebu_expression_highest* ptree);
extern struct zebu_expression_multiplicative* inc_zebu_expression_multiplicative(struct zebu_expression_multiplicative* ptree);
extern struct zebu_expression_prefix* inc_zebu_expression_prefix(struct zebu_expression_prefix* ptree);
extern struct zebu_regex* inc_zebu_regex(struct zebu_regex* ptree);
extern struct zebu_regex_concat* inc_zebu_regex_concat(struct zebu_regex_concat* ptree);
extern struct zebu_regex_high* inc_zebu_regex_high(struct zebu_regex_high* ptree);
extern struct zebu_regex_suffix* inc_zebu_regex_suffix(struct zebu_regex_suffix* ptree);
extern struct zebu_regex_union* inc_zebu_regex_union(struct zebu_regex_union* ptree);
extern struct zebu_root* inc_zebu_root(struct zebu_root* ptree);
extern struct zebu_token_rule* inc_zebu_token_rule(struct zebu_token_rule* ptree);


extern void free_zebu_token(struct zebu_token* token);
extern void free_zebu_$start(struct zebu_$start* ptree);

extern void free_zebu_charset(struct zebu_charset* ptree);

extern void free_zebu_charset_high(struct zebu_charset_high* ptree);

extern void free_zebu_charset_inter(struct zebu_charset_inter* ptree);

extern void free_zebu_charset_prefix(struct zebu_charset_prefix* ptree);

extern void free_zebu_charset_range(struct zebu_charset_range* ptree);

extern void free_zebu_charset_union(struct zebu_charset_union* ptree);

extern void free_zebu_expression(struct zebu_expression* ptree);

extern void free_zebu_expression_additive(struct zebu_expression_additive* ptree);

extern void free_zebu_expression_highest(struct zebu_expression_highest* ptree);

extern void free_zebu_expression_multiplicative(struct zebu_expression_multiplicative* ptree);

extern void free_zebu_expression_prefix(struct zebu_expression_prefix* ptree);

extern void free_zebu_regex(struct zebu_regex* ptree);

extern void free_zebu_regex_concat(struct zebu_regex_concat* ptree);

extern void free_zebu_regex_high(struct zebu_regex_high* ptree);

extern void free_zebu_regex_suffix(struct zebu_regex_suffix* ptree);

extern void free_zebu_regex_union(struct zebu_regex_union* ptree);

extern void free_zebu_root(struct zebu_root* ptree);

extern void free_zebu_token_rule(struct zebu_token_rule* ptree);



struct zebu_$start* zebu_parse(FILE* stream);
