
#include "lua.h"

#include "lapi.h"

#include "ltable.h"


LUA_API void lua_createtable (lua_State *L, int narray, int nrec) {
	Table *t;
	/*lua_lock(L);
	luaC_checkGC(L);*/
	t = luaH_new(L);
	sethvalue(L, L->top, t);
	api_incr_top(L);
	/*if (narray > 0 || nrec > 0)
		luaH_resize(L, t, narray, nrec);*/
	//lua_unlock(L);
}