#include "lua.h"
#include "lauxlib.h"

#include <stdlib.h>
#include <string.h>

#include <systemd/sd-login.h>

static int handle_error(lua_State *L, int err) {
	lua_pushnil(L);
	lua_pushstring(L, strerror(err));
	lua_pushinteger(L, err);
	return 3;
}

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

int luaopen_systemd_login_core (lua_State *L) {
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
        {NULL, NULL}
    };
    luaL_newlib(L, lib);

    return 1;
}

