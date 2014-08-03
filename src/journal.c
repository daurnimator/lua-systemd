#include "lua.h"
#include "lauxlib.h"
#include "compat-5.2.h"

#include <string.h>
#include <systemd/sd-journal.h>

static int handle_log_result(lua_State *L, int err) {
	if (err == 0) {
		lua_pushboolean(L, 1);
		return 1;
	} else {
		lua_pushnil(L);
		lua_pushstring(L, strerror(err));
		return 2;
	}
}

static int _perror (lua_State *L) {
	const char *message = luaL_checkstring(L, 1);
	return handle_log_result(L, sd_journal_perror(message));
}

int luaopen_systemd_journal_core (lua_State *L) {
	static const luaL_Reg lib[] = {
		{"perror", _perror},
		{NULL, NULL}
	};
	luaL_newlib(L, lib);

	lua_createtable(L, 0, 3);
	lua_pushnumber(L, SD_JOURNAL_NOP); lua_setfield(L, -2, "NOP");
	lua_pushnumber(L, SD_JOURNAL_APPEND); lua_setfield(L, -2, "APPEND");
	lua_pushnumber(L, SD_JOURNAL_INVALIDATE); lua_setfield(L, -2, "INVALIDATE");
	lua_setfield(L, -2, "wakeup");

	return 1;
}
