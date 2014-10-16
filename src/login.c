#include "lua.h"
#include "lauxlib.h"
#include "compat-5.2.h"

#include <stdlib.h>

#include <systemd/sd-login.h>

#include "util.c"
#include "login.h"

static int marshall_array_of_strings (lua_State *L, char ** strings, int n) {
	int i;
	lua_createtable(L, n, 0);
	for (i=0; i<n; i++) {
		lua_pushstring(L, strings[i]);
		free(strings[i]);
		lua_rawseti(L, -2, i+1);
	}
	free(strings);
	return 1;
}

static int marshall_array_of_uids (lua_State *L, uid_t * uids, int n) {
	int i;
	lua_createtable(L, n, 0);
	for (i=0; i<n; i++) {
		lua_pushinteger(L, uids[i]);
		lua_rawseti(L, -2, i+1);
	}
	free(uids);
	return 1;
}

static int get_seats (lua_State *L) {
	char **seats;
	int n = sd_get_seats(&seats);
	if (n < 0) return handle_error(L, -n);
	return marshall_array_of_strings(L, seats, n);
}

static int get_sessions (lua_State *L) {
	char **sessions;
	int n = sd_get_sessions(&sessions);
	if (n < 0) return handle_error(L, -n);
	return marshall_array_of_strings(L, sessions, n);
}

static int get_uids (lua_State *L) {
	uid_t *users;
	int n = sd_get_uids(&users);
	if (n < 0) return handle_error(L, -n);
	return marshall_array_of_uids(L, users, n);
}

static int get_machine_names (lua_State *L) {
	char **machines;
	int n = sd_get_machine_names(&machines);
	if (n < 0) return handle_error(L, -n);
	return marshall_array_of_strings(L, machines, n);
}

static int pid_get_session (lua_State *L) {
	pid_t pid = luaL_checkint(L, 1);
	char *session;
	int n = sd_pid_get_session(pid, &session);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, session);
	free(session);
	return 1;
}

static int pid_get_unit (lua_State *L) {
	pid_t pid = luaL_checkint(L, 1);
	char *unit;
	int n = sd_pid_get_unit(pid, &unit);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, unit);
	free(unit);
	return 1;
}

static int pid_get_user_unit (lua_State *L) {
	pid_t pid = luaL_checkint(L, 1);
	char *unit;
	int n = sd_pid_get_user_unit(pid, &unit);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, unit);
	free(unit);
	return 1;
}

static int pid_get_owner_uid (lua_State *L) {
	pid_t pid = luaL_checkint(L, 1);
	uid_t user;
	int n = sd_pid_get_owner_uid(pid, &user);
	if (n < 0) return handle_error(L, -n);
	lua_pushinteger(L, user);
	return 1;
}

static int pid_get_machine_name (lua_State *L) {
	pid_t pid = luaL_checkint(L, 1);
	char *machine;
	int n = sd_pid_get_machine_name(pid, &machine);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, machine);
	free(machine);
	return 1;
}

static int pid_get_slice (lua_State *L) {
	pid_t pid = luaL_checkint(L, 1);
	char *slice;
	int n = sd_pid_get_slice(pid, &slice);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, slice);
	free(slice);
	return 1;
}

static int peer_get_session (lua_State *L) {
	int fd = luaL_checkint(L, 1);
	char *session;
	int n = sd_peer_get_session(fd, &session);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, session);
	free(session);
	return 1;
}

static int peer_get_unit (lua_State *L) {
	int fd = luaL_checkint(L, 1);
	char *unit;
	int n = sd_peer_get_unit(fd, &unit);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, unit);
	free(unit);
	return 1;
}

static int peer_get_user_unit (lua_State *L) {
	int fd = luaL_checkint(L, 1);
	char *unit;
	int n = sd_peer_get_user_unit(fd, &unit);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, unit);
	free(unit);
	return 1;
}

static int peer_get_owner_uid (lua_State *L) {
	int fd = luaL_checkint(L, 1);
	uid_t user;
	int n = sd_peer_get_owner_uid(fd, &user);
	if (n < 0) return handle_error(L, -n);
	lua_pushinteger(L, user);
	return 1;
}

static int peer_get_machine_name (lua_State *L) {
	int fd = luaL_checkint(L, 1);
	char *machine;
	int n = sd_peer_get_machine_name(fd, &machine);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, machine);
	free(machine);
	return 1;
}

static int peer_get_slice (lua_State *L) {
	int fd = luaL_checkint(L, 1);
	char *slice;
	int n = sd_pid_get_slice(fd, &slice);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, slice);
	free(slice);
	return 1;
}


/* sd_login_monitor */

static sd_login_monitor* check_monitor(lua_State *L, int index) {
	sd_login_monitor **mp = luaL_checkudata(L, index, MONITOR_METATABLE);
	if (*mp == NULL) luaL_error(L, "Invalid monitor handle");
	return *mp;
}

static int monitor_unref (lua_State *L) {
	sd_login_monitor* m = check_monitor(L, 1);
	sd_login_monitor_unref(m);
	return 0;
}

static int monitor_new (lua_State *L) {
	const char* category = luaL_optstring(L, 1, NULL);
	sd_login_monitor** ret = lua_newuserdata(L, sizeof(sd_login_monitor*));
	int err = sd_login_monitor_new(category, ret);
	if (err < 0) return handle_error(L, -err);
	luaL_setmetatable(L, MONITOR_METATABLE);
	return 1;
}

static int monitor_tostring (lua_State *L) {
	lua_pushfstring(L, "%s: %p", MONITOR_METATABLE, lua_topointer(L, 1));
	return 1;
}

static int monitor_flush (lua_State *L) {
	sd_login_monitor* m = check_monitor(L, 1);
	int err = sd_login_monitor_flush(m);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, 1);
	return 1;
}

static int monitor_get_fd (lua_State *L) {
	sd_login_monitor* m = check_monitor(L, 1);
	int err = sd_login_monitor_get_fd(m);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, err);
	return 1;
}

static int monitor_get_events (lua_State *L) {
	sd_login_monitor* m = check_monitor(L, 1);
	int err = sd_login_monitor_get_events(m);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, err);
	return 1;
}

static int monitor_get_timeout (lua_State *L) {
	sd_login_monitor* m = check_monitor(L, 1);
	uint64_t timeout_usec;
	int err = sd_login_monitor_get_timeout(m, &timeout_usec);
	if (err < 0) return handle_error(L, -err);
	if ((err == 0) || (timeout_usec == (uint64_t) -1)) {
		lua_pushboolean(L, 0);
	} else {
		lua_pushnumber(L, ((double)timeout_usec)/1000000);
	}
	return 1;
}

int luaopen_systemd_login_core (lua_State *L) {
	static const luaL_Reg monitor_methods[] = {
		{"flush", monitor_flush},
		{"get_fd", monitor_get_fd},
		{"get_events", monitor_get_events},
		{"get_timeout", monitor_get_timeout},
		{NULL, NULL}
	};

	static const luaL_Reg lib[] = {
		{"get_seats", get_seats},
		{"get_sessions", get_sessions},
		{"get_uids", get_uids},
		{"get_machine_names", get_machine_names},
		{"pid_get_session", pid_get_session},
		{"pid_get_unit", pid_get_unit},
		{"pid_get_user_unit", pid_get_user_unit},
		{"pid_get_owner_uid", pid_get_owner_uid},
		{"pid_get_machine_name", pid_get_machine_name},
		{"pid_get_slice", pid_get_slice},
		{"peer_get_session", peer_get_session},
		{"peer_get_unit", peer_get_unit},
		{"peer_get_user_unit", peer_get_user_unit},
		{"peer_get_owner_uid", peer_get_owner_uid},
		{"peer_get_machine_name", peer_get_machine_name},
		{"peer_get_slice", peer_get_slice},
		{"monitor", monitor_new},
		{NULL, NULL}
	};
	luaL_newlib(L, lib);

	if (luaL_newmetatable(L, MONITOR_METATABLE) != 0) {
		luaL_newlib(L, monitor_methods);
		lua_setfield(L, -2, "__index");
		lua_pushcfunction(L, monitor_unref);
		lua_setfield(L, -2, "__gc");
		lua_pushcfunction(L, monitor_tostring);
		lua_setfield(L, -2, "__tostring");
	}
	/* Expose monitor methods */
	lua_getfield(L, -1, "__index");
	lua_setfield(L, -3, "MONITOR_METHODS");

	lua_pop(L, 1);

	return 1;
}
