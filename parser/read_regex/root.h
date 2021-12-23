
#include <inttypes.h>
#include <wchar.h>

enum token;

union tokendata;

struct memory_arena;
struct nfa_node;

int read_regex_root(
	int fd,
	uint8_t* cb,
	int (*rb)(int, uint8_t*),
	wchar_t* cc,
	enum token* ct,
	union tokendata* ctd,
	struct memory_arena* regex_arena,
	struct nfa_node** out_start,
	struct nfa_node** out_accept);
