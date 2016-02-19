#ifndef lua_h
#define lua_h


#include <stdarg.h>
#include <stddef.h>

#include "luaconf.h"

typedef struct lua_State lua_State;

/* thread status */
#define LUA_OK		0

/*
** basic types
*/
#define LUA_TNONE		(-1)

#define LUA_TNIL		0

#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5


#define LUA_TTHREAD		8

/* minimum Lua stack available to a C function */
#define LUA_MINSTACK	20


/*
** prototype for memory-allocation functions
*/
typedef void * (*lua_Alloc) (void *ud, void *ptr, size_t osize, size_t nsize);



/* type of numbers in Lua */
typedef LUA_NUMBER lua_Number;


/* type for integer functions */
typedef LUA_INTEGER lua_Integer;

/* unsigned integer type */
typedef LUA_UNSIGNED lua_Unsigned;

LUA_API lua_State *(lua_newstate) ();

LUA_API void  (lua_createtable) (lua_State *L, int narr, int nrec);

#define lua_newtable(L)		lua_createtable(L, 0, 0)


#endif