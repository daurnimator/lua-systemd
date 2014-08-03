#include "lua.h"
#include "lauxlib.h"
#include "compat-5.2.h"

#include <string.h>
#include <systemd/sd-daemon.h>

static int handle_notify_result (lua_State *L, int err) {
	if (err > 0) {
		lua_pushboolean(L, 1);
		return 1;
	} else {
		lua_pushnil(L);
		if (err == 0) {
			lua_pushliteral(L, "NOTIFY_SOCKET not set");
		} else {
			lua_pushstring(L, strerror(-err));
		}
		lua_pushinteger(L, -err);
		return 3;
	}
}

static int notify (lua_State *L) {
	int unset_environment = lua_toboolean(L, 1);
	const char *state = luaL_checkstring(L, 2);
	return handle_notify_result(L, sd_notify(unset_environment, state));
}

static int pid_notify (lua_State *L) {
	pid_t pid = luaL_checkint(L, 1);
	int unset_environment = lua_toboolean(L, 2);
	const char *state = luaL_checkstring(L, 3);
	return handle_notify_result(L, sd_pid_notify(pid, unset_environment, state));
}

static int booted (lua_State *L) {
	int booted = sd_booted();
	if (booted >= 0) {
		lua_pushboolean(L, booted);
		return 1;
	} else {
		lua_pushnil(L);
		lua_pushstring(L, strerror(-booted));
		lua_pushinteger(L, -booted);
		return 3;
	}
}

int luaopen_systemd_daemon_core (lua_State *L) {
	static const luaL_Reg lib[] = {
		{"notify", notify},
		{"pid_notify", pid_notify},
		{"booted", booted},
		{NULL, NULL}
	};
	luaL_newlib(L, lib);

	lua_pushnumber(L, SD_LISTEN_FDS_START); lua_setfield(L, -2, "LISTEN_FDS_START");

	return 1;
}
