#include "lua.h"
#include "lauxlib.h"
#include "compat-5.2.h"

#include <systemd/sd-daemon.h>

#include "util.c"

static int handle_notify_result (lua_State *L, int err) {
	if (err > 0) {
		lua_pushboolean(L, 1);
		return 1;
	} else if (err == 0) {
		lua_pushnil(L);
		lua_pushliteral(L, "NOTIFY_SOCKET not set");
		return 2;
	} else {
		return handle_error(L, -err);
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
		return handle_error(L, -booted);
	}
}

int luaopen_systemd_daemon_core (lua_State *L) {
	lua_newtable(L);
	if (systemd_has(209)) {
		set_func(L, notify, "notify");
		set_func(L, booted, "booted");
	}
	if (systemd_has(214)) {
		set_func(L, pid_notify, "pid_notify");
	}

	lua_pushnumber(L, SD_LISTEN_FDS_START); lua_setfield(L, -2, "LISTEN_FDS_START");

	return 1;
}
