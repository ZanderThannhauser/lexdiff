
#include <inttypes.h>
#include <wchar.h>

int utf8_decode(const uint8_t *utf8, wchar_t* codepoint)
	__attribute__ ((warn_unused_result));
