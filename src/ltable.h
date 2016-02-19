
#ifndef ltable_h
#define ltable_h


#include "lobject.h"


#define gnode(t,i)	(&(t)->node[i])
#define gkey(n)		(&(n)->i_key.tvk)
#define gval(n)		(&(n)->i_val)
#define gnext(n)	((n)->i_key.nk.next)


LUAI_FUNC TValue *luaH_newkey (lua_State *L, Table *t, const TValue *key);
LUAI_FUNC Table *luaH_new (lua_State *L);


#endif