
enum token;
union tokendata;
struct arena;
struct nfa_node;

int read_regex_root(
	int fd,
	uint8_t* cb,
	int (*rb)(int, uint8_t*),
	wchar_t* cc,
	enum token* ct,
	union tokendata* ctd,
	struct arena* regex_arena,
	struct nfa_node** out_start,
	struct nfa_node** out_accept,
	struct nfa_node** out_reject);
