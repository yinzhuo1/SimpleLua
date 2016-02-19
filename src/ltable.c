

#include <string.h>


#include "lua.h"

#include "ltable.h"
#include "lmem.h"
#include "lgc.h"


#define hashpow2(t,n)		(gnode(t, lmod((n), sizenode(t))))

#define hashstr(t,str)		hashpow2(t, (str)->tsv.hash)

/*
** for some types, it is better to avoid modulus by power of 2, as
** they tend to have many 2 factors.
*/
#define hashmod(t,n)	(gnode(t, ((n) % ((sizenode(t)-1)|1))))

#define dummynode		(&dummynode_)

#define isdummy(n)		((n) == dummynode)

static const Node dummynode_ = {
	{NILCONSTANT},  /* value */
	{{NILCONSTANT, NULL}}  /* key */
};


/*
** hash for lua_Numbers
*/
static Node *hashnum (const Table *t, lua_Number n) {
	int i;
	luai_hashnum(i, n);
	if (i < 0) {
		if (cast(unsigned int, i) == 0u - i)  /* use unsigned to avoid overflows */
			i = 0;  /* handle INT_MIN */
		i = -i;  /* must be a positive value */
	}
	return hashmod(t, i);
}

/*
** returns the `main' position of an element in a table (that is, the index
** of its hash value)
*/
static Node *mainposition (const Table *t, const TValue *key) {
	switch (ttype(key)) {
	case LUA_TNUMBER:
		return hashnum(t, nvalue(key));
	case LUA_TLNGSTR: {
		TString *s = rawtsvalue(key);
		if (s->tsv.extra == 0) {  /* no hash? */
			s->tsv.hash = luaS_hash(getstr(s), s->tsv.len, s->tsv.hash);
			s->tsv.extra = 1;  /* now it has its hash */
		}
		return hashstr(t, rawtsvalue(key));
					  }
	case LUA_TSHRSTR:
		return hashstr(t, rawtsvalue(key));
		/*case LUA_TBOOLEAN:
		return hashboolean(t, bvalue(key));
		case LUA_TLIGHTUSERDATA:
		return hashpointer(t, pvalue(key));
		case LUA_TLCF:
		return hashpointer(t, fvalue(key));
		default:
		return hashpointer(t, gcvalue(key));*/
	}
}


static void setnodevector (lua_State *L, Table *t, int size) {
	int lsize;
	if (size == 0) {  /* no elements to hash part? */
		t->node = cast(Node *, dummynode);  /* use common `dummynode' */
		lsize = 0;
	}
	else {
		int i;
		lsize = luaO_ceillog2(size);
		/*if (lsize > MAXBITS)
		luaG_runerror(L, "table overflow");*/
		size = twoto(lsize);
		t->node = luaM_newvector(L, size, Node);
		for (i=0; i<size; i++) {
			Node *n = gnode(t, i);
			gnext(n) = NULL;
			setnilvalue(gkey(n));
			setnilvalue(gval(n));
		}
	}
	t->lsizenode = cast_byte(lsize);
	t->lastfree = gnode(t, size);  /* all positions are free */
}


Table *luaH_new (lua_State *L) {
	Table *t = &luaC_newobj(L, LUA_TTABLE, sizeof(Table), NULL, 0)->h;
	t->metatable = NULL;
	t->flags = cast_byte(~0);
	t->array = NULL;
	t->sizearray = 0;
	setnodevector(L, t, 0);
	return t;
}

static Node *getfreepos (Table *t) {
	while (t->lastfree > t->node) {
		t->lastfree--;
		if (ttisnil(gkey(t->lastfree)))
			return t->lastfree;
	}
	return NULL;  /* could not find a free place */
}

/*
** inserts a new key into a hash table; first, check whether key's main
** position is free. If not, check whether colliding node is in its main
** position or not: if it is not, move colliding node to an empty place and
** put new key in its main position; otherwise (colliding node is in its main
** position), new key goes to an empty position.
*/
TValue *luaH_newkey (lua_State *L, Table *t, const TValue *key) {
	Node *mp;
	/*if (ttisnil(key)) luaG_runerror(L, "table index is nil");
	else if (ttisnumber(key) && luai_numisnan(L, nvalue(key)))
	luaG_runerror(L, "table index is NaN");*/
	mp = mainposition(t, key);
	if (!ttisnil(gval(mp)) || isdummy(mp)) {  /* main position is taken? */
		Node *othern;
		Node *n = getfreepos(t);  /* get a free place */
		if (n == NULL) {  /* cannot find a free place? */
			rehash(L, t, key);  /* grow table */
			/* whatever called 'newkey' take care of TM cache and GC barrier */
			return luaH_set(L, t, key);  /* insert key into grown table */
		}
		lua_assert(!isdummy(n));
		othern = mainposition(t, gkey(mp));
		if (othern != mp) {  /* is colliding node out of its main position? */
			/* yes; move colliding node into free position */
			while (gnext(othern) != mp) othern = gnext(othern);  /* find previous */
			gnext(othern) = n;  /* redo the chain with `n' in place of `mp' */
			*n = *mp;  /* copy colliding node into free pos. (mp->next also goes) */
			gnext(mp) = NULL;  /* now `mp' is free */
			setnilvalue(gval(mp));
		}
		else {  /* colliding node is in its own main position */
			/* new node will go into free position */
			gnext(n) = gnext(mp);  /* chain new position */
			gnext(mp) = n;
			mp = n;
		}
	}
	setobj2t(L, gkey(mp), key);
	luaC_barrierback(L, obj2gco(t), key);
	lua_assert(ttisnil(gval(mp)));
	return gval(mp);
}