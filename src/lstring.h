#ifndef lstring_h
#define lstring_h


#include "lobject.h"

LUAI_FUNC unsigned int luaS_hash (const char *str, size_t l, unsigned int seed);

#endif