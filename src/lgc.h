
#ifndef lgc_h
#define lgc_h


#include "lobject.h"
#include "lstate.h"


LUAI_FUNC GCObject *luaC_newobj (lua_State *L, int tt, size_t sz, GCObject **list, int offset);

#endif