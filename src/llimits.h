
#ifndef llimits_h
#define llimits_h


#include "lua.h"
#include <limits.h>


typedef unsigned LUA_INT32 lu_int32;

typedef LUAI_UMEM lu_mem;

typedef LUAI_MEM l_mem;

/* chars used as small naturals (so that `char' is reserved for characters) */
typedef unsigned char lu_byte;


/* type to ensure maximum alignment */
#if !defined(LUAI_USER_ALIGNMENT_T)
#define LUAI_USER_ALIGNMENT_T	union { double u; void *s; long l; }
#endif

typedef LUAI_USER_ALIGNMENT_T L_Umaxalign;


#define cast(t, exp)	((t)(exp))

#define cast_byte(i)	cast(lu_byte, (i))

/*
** type for virtual-machine instructions
** must be an unsigned with (at least) 4 bytes (see details in lopcodes.h)
*/
typedef lu_int32 Instruction;


#include <float.h>
#include <math.h>

#define lua_number2int(i,n)  __asm {__asm fld n   __asm fistp i}

#define luai_hashnum(i,n) { int e;  \
	n = frexp(n, &e) * (lua_Number)(INT_MAX - DBL_MAX_EXP);  \
	lua_number2int(i, n); i += e; }



#endif