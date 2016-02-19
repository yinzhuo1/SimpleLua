
#include <stddef.h>
#include <string.h>

#include "lstate.h"
#include "lua.h"
#include "lobject.h"
#include "lmem.h"

/*
** thread state + extra space
*/
typedef struct LX {
	lua_State l;
} LX;


/*
** Main thread combines a thread state and the global state
*/
typedef struct LG {
	LX l;
	global_State g;
} LG;

static void *l_alloc (void *ud, void *ptr, size_t osize, size_t nsize) {
	(void)ud; (void)osize;  /* not used */
	if (nsize == 0) {
		free(ptr);
		return NULL;
	}
	else
		return realloc(ptr, nsize);
}

/*
** preinitialize a state with consistent values without allocating
** any memory (to avoid errors)
*/
static void preinit_state (lua_State *L, global_State *g) {
	G(L) = g;
	L->stack = NULL;
	L->ci = NULL;
	L->stacksize = 0;
	//L->errorJmp = NULL;
	L->nCcalls = 0;
	//L->hook = NULL;
	//L->hookmask = 0;
	//L->basehookcount = 0;
	//L->allowhook = 1;
	//resethookcount(L);
	L->openupval = NULL;
	L->nny = 1;
	L->status = LUA_OK;
	L->errfunc = 0;
}

static void stack_init (lua_State *L1, lua_State *L) {
	int i; CallInfo *ci;
	/* initialize stack array */
	L1->stack = luaM_newvector(L, BASIC_STACK_SIZE, TValue);
	L1->stacksize = BASIC_STACK_SIZE;
	for (i = 0; i < BASIC_STACK_SIZE; i++)
		setnilvalue(L1->stack + i);  /* erase new stack */
	L1->top = L1->stack;
	L1->stack_last = L1->stack + L1->stacksize;
	/* initialize first ci */
	ci = &L1->base_ci;
	ci->next = ci->previous = NULL;
	ci->callstatus = 0;
	ci->func = L1->top;
	setnilvalue(L1->top++);  /* 'function' entry for this 'ci' */
	ci->top = L1->top + LUA_MINSTACK;
	L1->ci = ci;
}

/*
** open parts of the state that may cause memory-allocation errors
*/
static void f_luaopen (lua_State *L, void *ud) {
	global_State *g = G(L);
	//UNUSED(ud);
	stack_init(L, L);  /* init stack */
	//init_registry(L, g);
	//luaS_resize(L, MINSTRTABSIZE);  /* initial size of string table */
	//luaT_init(L);
	//luaX_init(L);
	///* pre-create memory-error message */
	//g->memerrmsg = luaS_newliteral(L, MEMERRMSG);
	//luaS_fix(g->memerrmsg);  /* it should never be collected */
	//g->gcrunning = 1;  /* allow gc */
}



LUA_API lua_State *lua_newstate () {
	int i;
	lua_State *L;
	global_State *g;
	LG *l = cast(LG *, l_alloc(NULL, NULL, LUA_TTHREAD, sizeof(LG)));
	if (l == NULL) return NULL;
	L = &l->l.l;
	g = &l->g;
	L->next = NULL;
	L->tt = LUA_TTHREAD;
	/*g->currentwhite = bit2mask(WHITE0BIT, FIXEDBIT);
	L->marked = luaC_white(g);
	g->gckind = KGC_NORMAL;*/
	preinit_state(L, g);
	g->frealloc = l_alloc;
	g->mainthread = L;
	//g->seed = makeseed(L);
	//g->uvhead.u.l.prev = &g->uvhead;
	//g->uvhead.u.l.next = &g->uvhead;
	//g->gcrunning = 0;  /* no GC while building state */
	//g->GCestimate = 0;
	g->strt.size = 0;
	g->strt.nuse = 0;
	g->strt.hash = NULL;
	setnilvalue(&g->l_registry);
	/*luaZ_initbuffer(L, &g->buff);
	g->panic = NULL;
	g->version = lua_version(NULL);
	g->gcstate = GCSpause;
	g->allgc = NULL;
	g->finobj = NULL;
	g->tobefnz = NULL;
	g->sweepgc = g->sweepfin = NULL;
	g->gray = g->grayagain = NULL;
	g->weak = g->ephemeron = g->allweak = NULL;*/
	g->totalbytes = sizeof(LG);
	/*g->GCdebt = 0;
	g->gcpause = LUAI_GCPAUSE;
	g->gcmajorinc = LUAI_GCMAJOR;
	g->gcstepmul = LUAI_GCMUL;
	for (i=0; i < LUA_NUMTAGS; i++) g->mt[i] = NULL;*/
	//if (luaD_rawrunprotected(L, f_luaopen, NULL) != LUA_OK) {
	//	/* memory allocation error: free partial state */
	//	close_state(L);
	//	L = NULL;
	//}
	//else
	//	luai_userstateopen(L);

	f_luaopen(L, NULL);

	return L;
}







