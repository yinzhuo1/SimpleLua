

#include "lgc.h"
#include "lmem.h"
#include "lobject.h"
#include "lstate.h"


/*
** create a new collectable object (with given type and size) and link
** it to '*list'. 'offset' tells how many bytes to allocate before the
** object itself (used only by states).
*/
GCObject *luaC_newobj (lua_State *L, int tt, size_t sz, GCObject **list, int offset) {
						   global_State *g = G(L);
						   char *raw = cast(char *, luaM_newobject(L, novariant(tt), sz));
						   GCObject *o = obj2gco(raw + offset);
						   //if (list == NULL)
							  // list = &g->allgc;  /* standard list for collectable objects */
						   //gch(o)->marked = luaC_white(g);
						   gch(o)->tt = tt;
						   /* gch(o)->next = *list;
						   *list = o;*/
						   return o;
}