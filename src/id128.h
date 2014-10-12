#include "lua.h"
#include "lauxlib.h"

#define ID128_METATABLE "ID128"

static sd_id128_t check_id128_t (lua_State *L, int arg) {
	sd_id128_t boot_id;
	switch(lua_type(L, arg)) {
		case LUA_TUSERDATA:
			return *(sd_id128_t*)luaL_checkudata(L, arg, ID128_METATABLE);
		case LUA_TSTRING:
			luaL_argcheck(L, sd_id128_from_string(lua_tostring(L, arg), &boot_id) == 0, arg, "string is not a valid id128");
			return boot_id;
		default:
			luaL_argerror(L, arg, lua_pushfstring(L, "id128 expected, got %s", luaL_typename(L, arg)));
			/* unreachable */
	}
}

int luaopen_systemd_id128_core (lua_State *L);
