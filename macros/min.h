
#define min(a, b) ({ \
	typeof(a) _a = (a), _b = (b), retval; \
	\
	if (_a < _b) \
		retval = _a; \
	else \
		retval = _b; \
	\
	retval; \
})

