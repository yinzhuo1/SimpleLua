#ifndef lobject_h
#define lobject_h

#include "lua.h"

#include "llimits.h"

/* Bit mark for collectable types */
#define BIT_ISCOLLECTABLE	(1 << 6)

/* mark a tag as collectable */
#define ctb(t)			((t) | BIT_ISCOLLECTABLE)

/* Variant tags for strings */
#define LUA_TSHRSTR	(LUA_TSTRING | (0 << 4))  /* short strings */
#define LUA_TLNGSTR	(LUA_TSTRING | (1 << 4))  /* long strings */

/*
** Union of all collectable objects
*/
typedef union GCObject GCObject;

/*
** Union of all Lua values
*/
typedef union Value Value;

typedef struct lua_TValue TValue;

/*
** Common Header for all collectable objects (in macro form, to be
** included in other objects)
*/
#define CommonHeader	GCObject *next; lu_byte tt; lu_byte marked


/*
** Common header in struct form
*/
typedef struct GCheader {
	CommonHeader;
} GCheader;



/* little endian */
#define TValuefields  \
union { struct { Value v__; int tt__; } i; double d__; } u
#define NILCONSTANT	{{{NULL}, tag2tt(LUA_TNIL)}}


#define NNMARK		0x7FF7A500
#define NNMASK		0x7FFFFF00

#define tag2tt(t)	(NNMARK | (t))



/* field-access macros */
#define v_(o)		((o)->u.i.v__)
#define d_(o)		((o)->u.d__)
#define tt_(o)		((o)->u.i.tt__)

/* correspondence with standard representation */
#undef val_
#define val_(o)		v_(o)
#undef num_
#define num_(o)		d_(o)


#define ttisnumber(o)	((tt_(o) & NNMASK) != NNMARK)
/* raw type tag of a TValue */
#define rttype(o)	(ttisnumber(o) ? LUA_TNUMBER : tt_(o) & 0xff)
/* type tag of a TValue (bits 0-3 for tags + variant bits 4-5) */
#define ttype(o)	(rttype(o) & 0x3F)
/* type tag of a TValue with no variants (bits 0-3) */
#define ttypenv(o)	(novariant(rttype(o)))

/* Macros to test type */
#define checktag(o,t)		(rttype(o) == (t))
#define checktype(o,t)		(ttypenv(o) == (t))
#define ttisnil(o)		checktag((o), LUA_TNIL)
#define ttisstring(o)		checktype((o), LUA_TSTRING)


/* Macros to set values */
//#define settt_(o,t)	((o)->tt_=(t))
#define settt_(o,t)	(tt_(o) = tag2tt(t))
#define rawtsvalue(o)	check_exp(ttisstring(o), &val_(o).gc->ts)


/* Macros to access values */
#define nvalue(o)	 num_(o)

/* tag with no variants (bits 0-3) */
#define novariant(x)	((x) & 0x0F)



#define setnilvalue(obj) settt_(obj, LUA_TNIL)

#define sethvalue(L,obj,x) \
{ TValue *io=(obj); \
	val_(io).gc=cast(GCObject *, (x)); settt_(io, ctb(LUA_TTABLE)); \
	/*checkliveness(G(L),io);*/ }


union Value {
	GCObject *gc;    /* collectable objects */
	void *p;         /* light userdata */
	int b;           /* booleans */
	//lua_CFunction f; /* light C functions */
	//numfield         /* numbers */
};


struct lua_TValue {
	TValuefields;
};

typedef TValue *StkId;  /* index to stack elements */


/*
** Header for string value; string bytes follow the end of this structure
*/
typedef union TString {
	L_Umaxalign dummy;  /* ensures maximum alignment for strings */
	struct {
		CommonHeader;
		lu_byte extra;  /* reserved words for short strings; "has hash" for longs */
		unsigned int hash;
		size_t len;  /* number of characters in string */
	} tsv;
} TString;


/*
** Tables
*/

typedef union TKey {
	struct {
		TValuefields;
		struct Node *next;  /* for chaining */
	} nk;
	TValue tvk;
} TKey;


typedef struct Node {
	TValue i_val;
	TKey i_key;
} Node;





typedef struct Table {
	CommonHeader;
	lu_byte flags;  /* 1<<p means tagmethod(p) is not present */
	lu_byte lsizenode;  /* log2 of size of `node' array */
	struct Table *metatable;
	TValue *array;  /* array part */
	Node *node;
	Node *lastfree;  /* any free position is before this position */
	GCObject *gclist;
	int sizearray;  /* size of `array' array */
} Table;




/*
** `module' operation for hashing (size is always a power of 2)
*/
#define lmod(s,size) \
	((cast(int, (s) & ((size)-1)))))


#define twoto(x)	(1<<(x))
#define sizenode(t)	(twoto((t)->lsizenode))

LUAI_FUNC int luaO_ceillog2 (unsigned int x);

#endif
