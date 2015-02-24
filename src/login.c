#include "lua.h"
#include "lauxlib.h"
#include "compat-5.2.h"

#include <stdlib.h>

#include <systemd/sd-login.h>

#include "util.c"
#include "login.h"

weak_ENOTSUP_define(sd_pid_get_session, pid_t pid, char **session)
weak_ENOTSUP_define(sd_pid_get_owner_uid, pid_t pid, uid_t *uid)
weak_ENOTSUP_define(sd_pid_get_unit, pid_t pid, char **unit)
weak_ENOTSUP_define(sd_pid_get_user_unit, pid_t pid, char **unit)
weak_ENOTSUP_define(sd_pid_get_machine_name, pid_t pid, char **machine)
weak_ENOTSUP_define(sd_pid_get_slice, pid_t pid, char **slice)
weak_ENOTSUP_define(sd_peer_get_session, int fd, char **session)
weak_ENOTSUP_define(sd_peer_get_owner_uid, int fd, uid_t *uid)
weak_ENOTSUP_define(sd_peer_get_unit, int fd, char **unit)
weak_ENOTSUP_define(sd_peer_get_user_unit, int fd, char **unit)
weak_ENOTSUP_define(sd_peer_get_machine_name, int fd, char **machine)
weak_ENOTSUP_define(sd_peer_get_slice, int fd, char **slice)
weak_ENOTSUP_define(sd_uid_get_state, uid_t uid, char **state)
weak_ENOTSUP_define(sd_uid_get_display, uid_t uid, char **session)
weak_ENOTSUP_define(sd_uid_is_on_seat, uid_t uid, int require_active, const char *seat)
weak_ENOTSUP_define(sd_uid_get_sessions, uid_t uid, int require_active, char ***sessions)
weak_ENOTSUP_define(sd_uid_get_seats, uid_t uid, int require_active, char ***seats)
weak_ENOTSUP_define(sd_session_is_active, const char *session)
weak_ENOTSUP_define(sd_session_is_remote, const char *session)
weak_ENOTSUP_define(sd_session_get_state, const char *session, char **state)
weak_ENOTSUP_define(sd_session_get_uid, const char *session, uid_t *uid)
weak_ENOTSUP_define(sd_session_get_seat, const char *session, char **seat)
weak_ENOTSUP_define(sd_session_get_service, const char *session, char **service)
weak_ENOTSUP_define(sd_session_get_type, const char *session, char **type)
weak_ENOTSUP_define(sd_session_get_class, const char *session, char **clazz)
weak_ENOTSUP_define(sd_session_get_display, const char *session, char **display)
weak_ENOTSUP_define(sd_session_get_remote_host, const char *session, char **remote_host)
weak_ENOTSUP_define(sd_session_get_remote_user, const char *session, char **remote_user)
weak_ENOTSUP_define(sd_session_get_tty, const char *session, char **display)
weak_ENOTSUP_define(sd_session_get_vt, const char *session, unsigned *vtnr)
weak_ENOTSUP_define(sd_seat_get_active, const char *seat, char **session, uid_t *uid)
weak_ENOTSUP_define(sd_seat_get_sessions, const char *seat, char ***sessions, uid_t **uid, unsigned *n_uids)
weak_ENOTSUP_define(sd_seat_can_multi_session, const char *seat)
weak_ENOTSUP_define(sd_seat_can_tty, const char *seat)
weak_ENOTSUP_define(sd_seat_can_graphical, const char *seat)
weak_ENOTSUP_define(sd_machine_get_class, const char *machine, char **clazz)
weak_ENOTSUP_define(sd_machine_get_ifindices, const char *machine, int **ifindices)
weak_ENOTSUP_define(sd_get_seats, char ***seats)
weak_ENOTSUP_define(sd_get_sessions, char ***sessions)
weak_ENOTSUP_define(sd_get_uids, uid_t **users)
weak_ENOTSUP_define(sd_get_machine_names, char ***machines)
weak_ENOTSUP_define(sd_login_monitor_new, const char *category, sd_login_monitor** ret)
weak_define(sd_login_monitor *, sd_login_monitor_unref, (sd_login_monitor *m), NULL)
weak_ENOTSUP_define(sd_login_monitor_flush, sd_login_monitor *m)
weak_ENOTSUP_define(sd_login_monitor_get_fd, sd_login_monitor *m)
weak_ENOTSUP_define(sd_login_monitor_get_events, sd_login_monitor *m)
weak_ENOTSUP_define(sd_login_monitor_get_timeout, sd_login_monitor *m, uint64_t *timeout_usec)

static void push_array_of_strings (lua_State *L, char **strings, int n) {
	int i;
	lua_createtable(L, n, 0);
	if (strings != NULL) {
		for (i=0; i<n; i++) {
			lua_pushstring(L, strings[i]);
			free(strings[i]);
			lua_rawseti(L, -2, i+1);
		}
		free(strings);
	}
}

static void push_array_of_uids (lua_State *L, uid_t *uids, int n) {
	int i;
	lua_createtable(L, n, 0);
	if (uids != NULL) {
		for (i=0; i<n; i++) {
			lua_pushinteger(L, uids[i]);
			lua_rawseti(L, -2, i+1);
		}
		free(uids);
	}
}

static int get_seats (lua_State *L) {
	char **seats;
	int n = sd_get_seats(&seats);
	if (n < 0) return handle_error(L, -n);
	push_array_of_strings(L, seats, n);
	return 1;
}

static int get_sessions (lua_State *L) {
	char **sessions;
	int n = sd_get_sessions(&sessions);
	if (n < 0) return handle_error(L, -n);
	push_array_of_strings(L, sessions, n);
	return 1;
}

static int get_uids (lua_State *L) {
	uid_t *users;
	int n = sd_get_uids(&users);
	if (n < 0) return handle_error(L, -n);
	push_array_of_uids(L, users, n);
	return 1;
}

static int get_machine_names (lua_State *L) {
	char **machines;
	int n = sd_get_machine_names(&machines);
	if (n < 0) return handle_error(L, -n);
	push_array_of_strings(L, machines, n);
	return 1;
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

static int uid_get_state (lua_State *L) {
	uid_t uid = luaL_checkint(L, 1);
	char *state;
	int err = sd_uid_get_state(uid, &state);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, state);
	free(state);
	return 1;
}

static int uid_get_seats (lua_State *L) {
	uid_t uid = luaL_checkint(L, 1);
	int require_active = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	char **seats;
	int err = sd_uid_get_seats(uid, require_active, &seats);
	if (err < 0) return handle_error(L, -err);
	push_array_of_strings(L, seats, err);
	return 1;
}

static int uid_is_on_seat (lua_State *L) {
	uid_t uid = luaL_checkint(L, 1);
	int require_active = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	const char *seat = luaL_checkstring(L, 3);
	int err = sd_uid_is_on_seat(uid, require_active, seat);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int uid_get_sessions (lua_State *L) {
	uid_t uid = luaL_checkint(L, 1);
	int require_active = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	char ** sessions;
	int err = sd_uid_get_sessions(uid, require_active, &sessions);
	if (err < 0) return handle_error(L, -err);
	push_array_of_strings(L, sessions, err);
	return 1;
}

static int uid_get_display (lua_State *L) {
	uid_t uid = luaL_checkint(L, 1);
	char *session;
	int err = sd_uid_get_display(uid, &session);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, session);
	free(session);
	return 1;
}

static int session_is_active (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	int err = sd_session_is_active(session);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int session_is_remote (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	int err = sd_session_is_remote(session);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int session_get_state (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *state;
	int err = sd_session_get_state(session, &state);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, state);
	free(state);
	return 1;
}

static int session_get_uid (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	uid_t uid;
	int err = sd_session_get_uid(session, &uid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, uid);
	return 1;
}

static int session_get_seat (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *seat;
	int err = sd_session_get_seat(session, &seat);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, seat);
	free(seat);
	return 1;
}

static int session_get_service (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *service;
	int err = sd_session_get_service(session, &service);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, service);
	free(service);
	return 1;
}

static int session_get_type (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *type;
	int err = sd_session_get_type(session, &type);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, type);
	free(type);
	return 1;
}

static int session_get_class (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *clazz;
	int err = sd_session_get_class(session, &clazz);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, clazz);
	free(clazz);
	return 1;
}

static int session_get_display (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *display;
	int err = sd_session_get_display(session, &display);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, display);
	free(display);
	return 1;
}

static int session_get_remote_host (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *remote_host;
	int err = sd_session_get_remote_host(session, &remote_host);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, remote_host);
	free(remote_host);
	return 1;
}

static int session_get_remote_user (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *remote_user;
	int err = sd_session_get_remote_user(session, &remote_user);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, remote_user);
	free(remote_user);
	return 1;
}

static int session_get_tty (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *display;
	int err = sd_session_get_tty(session, &display);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, display);
	free(display);
	return 1;
}

static int session_get_vt (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	unsigned vtnr;
	int err = sd_session_get_vt(session, &vtnr);
	if (err < 0) return handle_error(L, -err);
	lua_pushunsigned(L, vtnr);
	return 1;
}

static int seat_get_active (lua_State *L) {
	const char *seat = luaL_optstring(L, 1, NULL);
	char *session;
	uid_t uid;
	int err = sd_seat_get_active(seat, &session, &uid);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, session);
	lua_pushinteger(L, uid);
	return 2;
}

static int seat_get_sessions (lua_State *L) {
	const char *seat = luaL_optstring(L, 1, NULL);
	char **sessions;
	uid_t *uid;
	unsigned n_uids;
	int err = sd_seat_get_sessions(seat, &sessions, &uid, &n_uids);
	if (err < 0) return handle_error(L, -err);
	push_array_of_strings(L, sessions, err);
	push_array_of_uids(L, uid, n_uids);
	return 2;
}

static int seat_can_multi_session (lua_State *L) {
	const char *seat = luaL_optstring(L, 1, NULL);
	int err = sd_seat_can_multi_session(seat);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int seat_can_tty (lua_State *L) {
	const char *seat = luaL_optstring(L, 1, NULL);
	int err = sd_seat_can_tty(seat);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int seat_can_graphical (lua_State *L) {
	const char *seat = luaL_optstring(L, 1, NULL);
	int err = sd_seat_can_graphical(seat);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int machine_get_class (lua_State *L) {
	const char *machine = luaL_checkstring(L, 1);
	char *clazz;
	int err = sd_machine_get_class(machine, &clazz);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, clazz);
	free(clazz);
	return 1;
}

static int machine_get_ifindices (lua_State *L) {
	const char *machine = luaL_checkstring(L, 1);
	int *ifindices;
	int i;
	int err = sd_machine_get_ifindices(machine, &ifindices);
	if (err < 0) return handle_error(L, -err);
	lua_createtable(L, err, 0);
	if (ifindices != NULL) {
		for (i=0; i<err; i++) {
			lua_pushinteger(L, ifindices[i]);
			lua_rawseti(L, -2, i+1);
		}
		free(ifindices);
	}
	return 1;
}

/* sd_login_monitor */

static sd_login_monitor* check_monitor(lua_State *L, int index) {
	sd_login_monitor **mp = luaL_checkudata(L, index, MONITOR_METATABLE);
	if (*mp == NULL) luaL_error(L, "Invalid monitor handle");
	return *mp;
}

static int monitor_unref (lua_State *L) {
	sd_login_monitor **mp = luaL_checkudata(L, 1, MONITOR_METATABLE);
	if (*mp != NULL) {
		sd_login_monitor_unref(*mp);
		*mp = NULL;
	}
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
	sd_login_monitor *m = check_monitor(L, 1);
	lua_pushfstring(L, "%s: %p", MONITOR_METATABLE, m);
	return 1;
}

static int monitor_flush (lua_State *L) {
	sd_login_monitor *m = check_monitor(L, 1);
	int err = sd_login_monitor_flush(m);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, 1);
	return 1;
}

static int monitor_get_fd (lua_State *L) {
	sd_login_monitor *m = check_monitor(L, 1);
	int err = sd_login_monitor_get_fd(m);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, err);
	return 1;
}

static int monitor_get_events (lua_State *L) {
	sd_login_monitor *m = check_monitor(L, 1);
	int err = sd_login_monitor_get_events(m);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, err);
	return 1;
}

static int monitor_get_timeout (lua_State *L) {
	sd_login_monitor *m = check_monitor(L, 1);
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
	lua_newtable(L);
	if (systemd_has(209)) {
		if (luaL_newmetatable(L, MONITOR_METATABLE) != 0) {
			lua_newtable(L);
			set_func(L, monitor_flush, "flush");
			set_func(L, monitor_get_fd, "get_fd");
			set_func(L, monitor_get_events,"get_events");
			set_func(L, monitor_get_timeout, "get_timeout");
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

		set_func(L, monitor_new, "monitor");

		set_func(L, pid_get_session, "pid_get_session");
		set_func(L, pid_get_unit, "pid_get_unit");
		set_func(L, pid_get_user_unit, "pid_get_user_unit");
		set_func(L, pid_get_owner_uid, "pid_get_owner_uid");
		set_func(L, pid_get_machine_name, "pid_get_machine_name");
		set_func(L, pid_get_slice, "pid_get_slice");

		set_func(L, uid_get_state, "uid_get_state");
		set_func(L, uid_is_on_seat, "uid_is_on_seat");
		set_func(L, uid_get_sessions, "uid_get_sessions");
		set_func(L, uid_get_seats, "uid_get_seats");

		set_func(L, session_is_active, "session_is_active");
		set_func(L, session_is_remote, "session_is_remote");
		set_func(L, session_get_state, "session_get_state");
		set_func(L, session_get_uid, "session_get_uid");
		set_func(L, session_get_seat, "session_get_seat");
		set_func(L, session_get_service, "session_get_service");
		set_func(L, session_get_type, "session_get_type");
		set_func(L, session_get_class, "session_get_class");
		set_func(L, session_get_display, "session_get_display");
		set_func(L, session_get_remote_host, "session_get_remote_host");
		set_func(L, session_get_remote_user, "session_get_remote_user");
		set_func(L, session_get_tty, "session_get_tty");
		set_func(L, session_get_vt, "session_get_vt");

		set_func(L, seat_get_active, "seat_get_active");
		set_func(L, seat_get_sessions, "seat_get_sessions");
		set_func(L, seat_can_multi_session, "seat_can_multi_session");
		set_func(L, seat_can_tty, "seat_can_tty");
		set_func(L, seat_can_graphical, "seat_can_graphical");

		set_func(L, get_seats, "get_seats");
		set_func(L, get_sessions, "get_sessions");
		set_func(L, get_uids, "get_uids");
		set_func(L, get_machine_names, "get_machine_names");
	}
	if (systemd_has(211)) {
		set_func(L, machine_get_class, "machine_get_class");
		set_func(L, peer_get_session, "peer_get_session");
		set_func(L, peer_get_owner_uid, "peer_get_owner_uid");
		set_func(L, peer_get_unit, "peer_get_unit");
		set_func(L, peer_get_user_unit, "peer_get_user_unit");
		set_func(L, peer_get_machine_name, "peer_get_machine_name");
		set_func(L, peer_get_slice, "peer_get_slice");
	}
	if (systemd_has(213)) {
		set_func(L, uid_get_display, "sd_uid_get_display");
	}
	if (systemd_has(216)) {
		set_func(L, machine_get_ifindices, "machine_get_ifindices");
	}

	return 1;
}
