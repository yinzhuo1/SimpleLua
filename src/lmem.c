
#include "lua.h"
#include "lmem.h"

#include "lobject.h"
#include "lstate.h"





/*
** generic allocation routine.
*/
void *luaM_realloc_ (lua_State *L, void *block, size_t osize, size_t nsize) {
	void *newblock;
	global_State *g = G(L);
	size_t realosize = (block) ? osize : 0;
	newblock = (*g->frealloc)(NULL, block, osize, nsize);

	return newblock;
}
