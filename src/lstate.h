#ifndef lstate_h
#define lstate_h


#include "lua.h"

#include "lobject.h"


#define BASIC_STACK_SIZE        (2*LUA_MINSTACK)


typedef struct stringtable {
	GCObject **hash;
	lu_int32 nuse;  /* number of elements */
	int size;
} stringtable;

/*
** information about a call
*/
typedef struct CallInfo {
	StkId func;  /* function index in the stack */
	StkId	top;  /* top for this function */
	struct CallInfo *previous, *next;  /* dynamic call link */
	short nresults;  /* expected number of results from this function */
	lu_byte callstatus;
	ptrdiff_t extra;
	union {
		struct {  /* only for Lua functions */
			StkId base;  /* base for this function */
			const Instruction *savedpc;
		} l;
		//struct {  /* only for C functions */
		//	int ctx;  /* context info. in case of yields */
		//	lua_CFunction k;  /* continuation in case of yields */
		//	ptrdiff_t old_errfunc;
		//	lu_byte old_allowhook;
		//	lu_byte status;
		//} c;
	} u;
} CallInfo;



/*
** `global state', shared by all threads of this state
*/
typedef struct global_State {
	lua_Alloc frealloc;  /* function to reallocate memory */
	lu_mem totalbytes;  /* number of bytes currently allocated - GCdebt */
	//l_mem GCdebt;  /* bytes allocated not yet compensated by the collector */
	//lu_mem GCmemtrav;  /* memory traversed by the GC */
	//lu_mem GCestimate;  /* an estimate of the non-garbage memory in use */
	stringtable strt;  /* hash table for strings */
	TValue l_registry;
	unsigned int seed;  /* randomized seed for hashes */
	//lu_byte currentwhite;
	//lu_byte gcstate;  /* state of garbage collector */
	//lu_byte gckind;  /* kind of GC running */
	//lu_byte gcrunning;  /* true if GC is running */
	//int sweepstrgc;  /* position of sweep in `strt' */
	//GCObject *allgc;  /* list of all collectable objects */
	//GCObject *finobj;  /* list of collectable objects with finalizers */
	//GCObject **sweepgc;  /* current position of sweep in list 'allgc' */
	//GCObject **sweepfin;  /* current position of sweep in list 'finobj' */
	//GCObject *gray;  /* list of gray objects */
	//GCObject *grayagain;  /* list of objects to be traversed atomically */
	//GCObject *weak;  /* list of tables with weak values */
	//GCObject *ephemeron;  /* list of ephemeron tables (weak keys) */
	//GCObject *allweak;  /* list of all-weak tables */
	//GCObject *tobefnz;  /* list of userdata to be GC */
	//UpVal uvhead;  /* head of double-linked list of all open upvalues */
	//Mbuffer buff;  /* temporary buffer for string concatenation */
	//int gcpause;  /* size of pause between successive GCs */
	//int gcmajorinc;  /* pause between major collections (only in gen. mode) */
	//int gcstepmul;  /* GC `granularity' */
	//lua_CFunction panic;  /* to be called in unprotected errors */
	struct lua_State *mainthread;
	//const lua_Number *version;  /* pointer to version number */
	//TString *memerrmsg;  /* memory-error message */
	//TString *tmname[TM_N];  /* array with tag-method names */
	//struct Table *mt[LUA_NUMTAGS];  /* metatables for basic types */
} global_State;


/*
** `per thread' state
*/
struct lua_State {
	CommonHeader;
	lu_byte status;
	StkId top;  /* first free slot in the stack */
	global_State *l_G;
	CallInfo *ci;  /* call info for current function */
	const Instruction *oldpc;  /* last pc traced */
	StkId stack_last;  /* last free slot in the stack */
	StkId stack;  /* stack base */
	int stacksize;
	unsigned short nny;  /* number of non-yieldable calls in stack */
	unsigned short nCcalls;  /* number of nested C calls */
	lu_byte hookmask;
	lu_byte allowhook;
	int basehookcount;
	int hookcount;
	//lua_Hook hook;
	GCObject *openupval;  /* list of open upvalues in this stack */
	GCObject *gclist;
	//struct lua_longjmp *errorJmp;  /* current error recover point */
	ptrdiff_t errfunc;  /* current error handling function (stack index) */
	CallInfo base_ci;  /* CallInfo for first level (C calling Lua) */
};

#define G(L)	(L->l_G)


/*
** Union of all collectable objects
*/
union GCObject {
	GCheader gch;  /* common header */
	union TString ts;
	//union Udata u;
	//union Closure cl;
	struct Table h;
	//struct Proto p;
	//struct UpVal uv;
	//struct lua_State th;  /* thread */
};


#define gch(o)		(&(o)->gch)

/* macro to convert any Lua object into a GCObject */
#define obj2gco(v)	(cast(GCObject *, (v)))

#endif