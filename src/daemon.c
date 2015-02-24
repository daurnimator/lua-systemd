#include "lua.h"
#include "lauxlib.h"
#include "compat-5.3.h"

#include <systemd/sd-daemon.h>

#include "util.c"

weak_ENOTSUP_define(sd_booted)
weak_ENOTSUP_define(sd_notify, int unset_environment, const char *state)
weak_ENOTSUP_define(sd_pid_notify, pid_t pid, int unset_environment, const char *state)
weak_ENOTSUP_define(sd_pid_notify_with_fds, pid_t pid, int unset_environment, const char *state, const int *fds, unsigned n_fds)

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
	pid_t pid = luaL_checkinteger(L, 1);
	int unset_environment = lua_toboolean(L, 2);
	const char *state = luaL_checkstring(L, 3);
	return handle_notify_result(L, sd_pid_notify(pid, unset_environment, state));
}

static int pid_notify_with_fds (lua_State *L) {
	pid_t pid = luaL_checkint(L, 1);
	int unset_environment = lua_toboolean(L, 2);
	const char *state = luaL_checkstring(L, 3);
	int *fds;
	unsigned n_fds;
	int i;
	luaL_checktype(L, 4, LUA_TTABLE);
	n_fds = lua_rawlen(L, 4);
	fds = lua_newuserdata(L, n_fds*sizeof(int));
	for (i=0; i < n_fds; i++) {
		lua_rawgeti(L, 4, i+1);
		luaL_argcheck(L, lua_type(L, -1) == LUA_TNUMBER && lua_isinteger(L, -1), 4, "expected array of file descritors (integers)");
		fds[i] = lua_tointeger(L, -1);
		lua_settop(L, 5);
	}
	return handle_notify_result(L, sd_pid_notify_with_fds(pid, unset_environment, state, fds, n_fds));
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
	if (systemd_has(219)) {
		set_func(L, pid_notify, "pid_notify_with_fds");
	}

	lua_pushnumber(L, SD_LISTEN_FDS_START); lua_setfield(L, -2, "LISTEN_FDS_START");

	return 1;
}
