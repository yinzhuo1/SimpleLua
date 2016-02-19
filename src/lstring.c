
#include <string.h>

#include "lua.h"

#include "lmem.h"
#include "lobject.h"
#include "lstate.h"
#include "lstring.h"


/*
** Lua will use at most ~(2^LUAI_HASHLIMIT) bytes from a string to
** compute its hash
*/
#if !defined(LUAI_HASHLIMIT)
#define LUAI_HASHLIMIT		5
#endif


unsigned int luaS_hash (const char *str, size_t l, unsigned int seed) {
	unsigned int h = seed ^ cast(unsigned int, l);
	size_t l1;
	size_t step = (l >> LUAI_HASHLIMIT) + 1;
	for (l1 = l; l1 >= step; l1 -= step)
		h = h ^ ((h<<5) + (h>>2) + cast_byte(str[l1 - 1]));
	return h;
}