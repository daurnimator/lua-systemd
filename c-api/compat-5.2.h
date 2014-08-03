#include "lua.h"
#include "lauxlib.h"

#if !defined(LUA_VERSION_NUM)
/* Lua 5.0 */

#define LUA_QL(x) "'" x "'"
#define LUA_QS LUA_QL("%s")

#define luaL_Reg luaL_reg

#define luaL_opt(L, f, n, d) \
  (lua_isnoneornil(L, n) ? (d) : f(L, n))

#define luaL_addchar(B,c) \
  ((void)((B)->p < ((B)->buffer+LUAL_BUFFERSIZE) || luaL_prepbuffer(B)), \
   (*(B)->p++ = (char)(c)))

#endif /* Lua 5.0 */


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM == 501
/* Lua 5.1 */

typedef LUAI_UINT32 lua_Unsigned;

#define lua_tounsigned(L, i) lua_tounsignedx(L, i, NULL)

#define lua_rawlen(L, i) lua_objlen(L, i)

void lua_pushunsigned (lua_State *L, lua_Unsigned n);
lua_Unsigned luaL_checkunsigned (lua_State *L, int i);
lua_Unsigned lua_tounsignedx (lua_State *L, int i, int *isnum);
lua_Unsigned luaL_optunsigned (lua_State *L, int i, lua_Unsigned def);
lua_Integer lua_tointegerx (lua_State *L, int i, int *isnum);
void lua_len (lua_State *L, int i);
int luaL_len (lua_State *L, int i);
const char *luaL_tolstring (lua_State *L, int idx, size_t *len);

#endif /* Lua 5.1 */


#if !defined(LUA_VERSION_NUM) || LUA_VERSION_NUM == 501
/* Lua 5.0 *or* 5.1 */

#define lua_pushglobaltable(L) \
  lua_pushvalue(L, LUA_GLOBALSINDEX)

#define luaL_newlib(L, l) \
  (lua_newtable((L)),luaL_setfuncs((L), (l), 0))

void luaL_checkversion (lua_State *L);

#endif /* Lua 5.0 *or* 5.1 */

int lua_absindex (lua_State *L, int i);
void lua_copy (lua_State *L, int from, int to);
void lua_rawgetp (lua_State *L, int i, const void *p);
void lua_rawsetp (lua_State *L, int i, const void *p);
void *luaL_testudata (lua_State *L, int i, const char *tname);
lua_Number lua_tonumberx (lua_State *L, int i, int *isnum);
void lua_getuservalue (lua_State *L, int i);
void lua_setuservalue (lua_State *L, int i);
void luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup);
void luaL_setmetatable (lua_State *L, const char *tname);
int luaL_getsubtable (lua_State *L, int i, const char *name);
void luaL_traceback (lua_State *L, lua_State *L1, const char *msg, int level);
int luaL_fileresult (lua_State *L, int stat, const char *fname);

