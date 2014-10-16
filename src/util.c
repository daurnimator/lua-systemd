#include <string.h>

static int handle_error(lua_State *L, int err) {
	lua_pushnil(L);
	lua_pushstring(L, strerror(err));
	lua_pushinteger(L, err);
	return 3;
}

#ifndef lua_pushuint64
#define lua_pushuint64 lua_pushnumber
#endif

#ifndef luaL_checkuint64
#define luaL_checkuint64 luaL_checknumber
#endif
