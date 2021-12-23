
#define cmp(a, b) ({ typeof(a) _a = (a), _b = (b); _a > _b ? 1 : _a < _b ? -1 : 0; })

