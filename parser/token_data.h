
#include <wchar.h>

union tokendata
{
	struct {
		wchar_t* data;
		size_t len;
	} string;
	
	struct {
		wchar_t codepoint;
	} character;
	
	double numeric;
};

