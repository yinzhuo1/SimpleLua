#ifndef lmem_h
#define lmem_h


#include "llimits.h"
#include "lua.h"


/*
** This macro avoids the runtime division MAX_SIZET/(e), as 'e' is
** always constant.
** The macro is somewhat complex to avoid warnings:
** +1 avoids warnings of "comparison has constant result";
** cast to 'void' avoids warnings of "value unused".
*/
#define luaM_reallocv(L,b,on,n,e) \
	(luaM_realloc_(L, (b), (on)*(e), (n)*(e)))


#define luaM_newvector(L,n,t) \
	cast(t *, luaM_reallocv(L, NULL, 0, n, sizeof(t)))

#define luaM_newobject(L,tag,s)	luaM_realloc_(L, NULL, tag, (s))




/* not to be called directly */
LUAI_FUNC void *luaM_realloc_ (lua_State *L, void *block, size_t oldsize, size_t size);

#endif