

#ifndef lconfig_h
#define lconfig_h


/*
@@ LUA_INTEGER is the integral type used by lua_pushinteger/lua_tointeger.
** CHANGE that if ptrdiff_t is not adequate on your machine. (On most
** machines, ptrdiff_t gives a good choice between int or long.)
*/
#define LUA_INTEGER	ptrdiff_t

/*
@@ LUA_UNSIGNED is the integral type used by lua_pushunsigned/lua_tounsigned.
** It must have at least 32 bits.
*/
#define LUA_UNSIGNED	unsigned LUA_INT32

#define LUA_INT32	int
#define LUAI_UMEM	size_t
#define LUAI_MEM	ptrdiff_t


#define LUA_NUMBER	double


#define LUA_API		extern
#define LUAI_FUNC	extern

#endif
