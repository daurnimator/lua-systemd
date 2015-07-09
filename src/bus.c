#include "lua.h"
#include "lauxlib.h"
#include "compat-5.3.h"

#include <errno.h> /* ENOTSUP */
#include <math.h> /* ceil, HUGE_VAL */

#include <systemd/sd-bus.h>
#include <systemd/sd-id128.h> /* SD_ID128_NULL */

#include "util.c"
#include "bus.h"
#include "id128.h"

static int tostring(lua_State *L) {
	lua_pushfstring(L, "%s: %p", lua_getmetatable(L, 1)?(lua_getfield(L, -1, "__name"), lua_tostring(L, -1)):"userdata", lua_topointer(L, 1));
	return 1;
}

/* Connections */

shim_weak_stub_declare(int, sd_bus_default, (sd_bus **ret), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_default_user, (sd_bus **ret), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_default_system, (sd_bus **ret), -ENOTSUP)

shim_weak_stub_declare(int, sd_bus_open, (sd_bus **ret), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_open_user, (sd_bus **ret), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_open_system, (sd_bus **ret), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_open_system_remote, (sd_bus **ret, const char *host), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_open_system_machine, (sd_bus **ret, const char *machine), -ENOTSUP)

shim_weak_stub_declare(int, sd_bus_new, (sd_bus **ret), -ENOTSUP)

shim_weak_stub_declare(int, sd_bus_set_address, (sd_bus *bus, const char *address), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_get_address, (sd_bus *bus, const char **address), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_set_bus_client, (sd_bus *bus, int b), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_is_bus_client, (sd_bus *bus), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_set_server, (sd_bus *bus, int b, sd_id128_t bus_id), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_is_server, (sd_bus *bus), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_set_anonymous, (sd_bus *bus, int b), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_is_anonymous, (sd_bus *bus), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_set_trusted, (sd_bus *bus, int b), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_is_trusted, (sd_bus *bus), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_set_monitor, (sd_bus *bus, int b), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_is_monitor, (sd_bus *bus), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_set_description, (sd_bus *bus, const char *description), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_get_description, (sd_bus *bus, const char **description), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_negotiate_creds, (sd_bus *bus, int b, uint64_t creds_mask), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_negotiate_timestamp, (sd_bus *bus, int b), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_negotiate_fds, (sd_bus *bus, int b), -ENOTSUP)

shim_weak_stub_declare(int, sd_bus_start, (sd_bus *ret), -ENOTSUP)

shim_weak_stub_declare(sd_bus*, sd_bus_ref, (sd_bus *bus), NULL)
shim_weak_stub_declare(sd_bus*, sd_bus_unref, (sd_bus *bus), NULL)

shim_weak_stub_declare(int, sd_bus_is_open, (sd_bus *bus), -ENOTSUP)

shim_weak_stub_declare(int, sd_bus_get_bus_id, (sd_bus *bus, sd_id128_t *id), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_get_scope, (sd_bus *bus, const char **scope), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_get_tid, (sd_bus *bus, pid_t *tid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_get_owner_creds, (sd_bus *bus, uint64_t creds_mask, sd_bus_creds **ret), -ENOTSUP)

shim_weak_stub_declare(int, sd_bus_send, (sd_bus *bus, sd_bus_message *m, uint64_t *cookie), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_send_to, (sd_bus *bus, sd_bus_message *m, const char *destination, uint64_t *cookie), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_call, (sd_bus *bus, sd_bus_message *m, uint64_t usec, sd_bus_error *ret_error, sd_bus_message **reply), -ENOTSUP)

shim_weak_stub_declare(int, sd_bus_get_fd, (sd_bus *bus), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_get_events, (sd_bus *bus), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_get_timeout, (sd_bus *bus, uint64_t *timeout_usec), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_process, (sd_bus *bus, sd_bus_message **r), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_process_priority, (sd_bus *bus, int64_t max_priority, sd_bus_message **r), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_wait, (sd_bus *bus, uint64_t timeout_usec), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_flush, (sd_bus *bus), -ENOTSUP)

shim_weak_stub_declare(sd_bus_slot*, sd_bus_get_current_slot, (sd_bus *bus), NULL)

/* Slot object */

shim_weak_stub_declare(sd_bus_slot*, sd_bus_slot_ref, (sd_bus_slot *slot), NULL)
shim_weak_stub_declare(sd_bus_slot*, sd_bus_slot_unref, (sd_bus_slot *slot), NULL)

shim_weak_stub_declare(sd_bus*, sd_bus_slot_get_bus, (sd_bus_slot *slot), NULL)

/* Message object */

shim_weak_stub_declare(int, sd_bus_message_new_signal, (sd_bus *bus, sd_bus_message **m, const char *path, const char *interface, const char *member), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_message_new_method_call, (sd_bus *bus, sd_bus_message **m, const char *destination, const char *path, const char *interface, const char *member), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_message_new_method_return, (sd_bus_message *call, sd_bus_message **m), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_message_new_method_error, (sd_bus_message *call, sd_bus_message **m, const sd_bus_error *e), -ENOTSUP)
shim_weak_stub_declare(sd_bus_message*, sd_bus_message_unref, (sd_bus_message *m), NULL)

shim_weak_stub_declare(const char*, sd_bus_message_get_signature, (sd_bus_message *m, int complete), NULL)
shim_weak_stub_declare(const char*, sd_bus_message_get_path, (sd_bus_message *m), NULL)
shim_weak_stub_declare(const char*, sd_bus_message_get_interface, (sd_bus_message *m), NULL)
shim_weak_stub_declare(const char*, sd_bus_message_get_member, (sd_bus_message *m), NULL)
shim_weak_stub_declare(const char*, sd_bus_message_get_destination, (sd_bus_message *m), NULL)
shim_weak_stub_declare(const char*, sd_bus_message_get_sender, (sd_bus_message *m), NULL)

shim_weak_stub_declare(sd_bus*, sd_bus_message_get_bus, (sd_bus_message *m), NULL)
shim_weak_stub_declare(sd_bus_creds*, sd_bus_message_get_creds, (sd_bus_message *m), NULL)

/* Credential handling */

shim_weak_stub_declare(sd_bus_creds*, sd_bus_creds_ref, (sd_bus_creds *c), NULL)
shim_weak_stub_declare(sd_bus_creds*, sd_bus_creds_unref, (sd_bus_creds *c), NULL)

shim_weak_stub_declare(int, sd_bus_creds_get_pid, (sd_bus_creds *c, pid_t *pid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_ppid, (sd_bus_creds *c, pid_t *ppid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_tid, (sd_bus_creds *c, pid_t *tid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_uid, (sd_bus_creds *c, uid_t *uid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_euid, (sd_bus_creds *c, uid_t *euid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_suid, (sd_bus_creds *c, uid_t *suid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_fsuid, (sd_bus_creds *c, uid_t *fsuid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_gid, (sd_bus_creds *c, gid_t *gid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_egid, (sd_bus_creds *c, gid_t *egid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_sgid, (sd_bus_creds *c, gid_t *sgid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_fsgid, (sd_bus_creds *c, gid_t *fsgid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_supplementary_gids, (sd_bus_creds *c, const gid_t **gids), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_comm, (sd_bus_creds *c, const char **comm), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_tid_comm, (sd_bus_creds *c, const char **comm), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_exe, (sd_bus_creds *c, const char **exe), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_cmdline, (sd_bus_creds *c, char ***cmdline), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_cgroup, (sd_bus_creds *c, const char **cgroup), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_unit, (sd_bus_creds *c, const char **unit), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_slice, (sd_bus_creds *c, const char **slice), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_user_unit, (sd_bus_creds *c, const char **unit), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_user_slice, (sd_bus_creds *c, const char **slice), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_session, (sd_bus_creds *c, const char **session), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_owner_uid, (sd_bus_creds *c, uid_t *uid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_has_effective_cap, (sd_bus_creds *c, int capability), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_has_permitted_cap, (sd_bus_creds *c, int capability), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_has_inheritable_cap, (sd_bus_creds *c, int capability), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_has_bounding_cap, (sd_bus_creds *c, int capability), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_selinux_context, (sd_bus_creds *c, const char **context), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_audit_session_id, (sd_bus_creds *c, uint32_t *sessionid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_audit_login_uid, (sd_bus_creds *c, uid_t *loginuid), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_tty, (sd_bus_creds *c, const char **tty), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_unique_name, (sd_bus_creds *c, const char **name), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_well_known_names, (sd_bus_creds *c, char ***names), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_creds_get_description, (sd_bus_creds *c, const char **name), -ENOTSUP)

/* Error structures */

shim_weak_stub_declare(void, sd_bus_error_free, (sd_bus_error *e), /* void */)
shim_weak_stub_declare(int, sd_bus_error_get_errno, (const sd_bus_error *e), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_error_is_set, (const sd_bus_error *e), -ENOTSUP)
shim_weak_stub_declare(int, sd_bus_error_has_name, (const sd_bus_error *e, const char *name), -ENOTSUP)


static int bus_default(lua_State *L) {
	sd_bus **bus = lua_newuserdata(L, sizeof(sd_bus*));
	int err = shim_weak_stub(sd_bus_default)(bus);
	if (err < 0) return handle_error(L, -err);
	if (cache_pointer(L, BUS_CACHE_KEY, *bus)) {
		luaL_setmetatable(L, BUS_METATABLE);
	} else {
		shim_weak_stub(sd_bus_unref)(*bus);
	}
	return 1;
}

static int bus_default_user(lua_State *L) {
	sd_bus **bus = lua_newuserdata(L, sizeof(sd_bus*));
	int err = shim_weak_stub(sd_bus_default_user)(bus);
	if (err < 0) return handle_error(L, -err);
	if (cache_pointer(L, BUS_CACHE_KEY, *bus)) {
		luaL_setmetatable(L, BUS_METATABLE);
	} else {
		shim_weak_stub(sd_bus_unref)(*bus);
	}
	return 1;
}

static int bus_default_system(lua_State *L) {
	sd_bus **bus = lua_newuserdata(L, sizeof(sd_bus*));
	int err = shim_weak_stub(sd_bus_default_system)(bus);
	if (err < 0) return handle_error(L, -err);
	if (cache_pointer(L, BUS_CACHE_KEY, *bus)) {
		luaL_setmetatable(L, BUS_METATABLE);
	} else {
		shim_weak_stub(sd_bus_unref)(*bus);
	}
	return 1;
}

static int bus_open(lua_State *L) {
	sd_bus **bus = lua_newuserdata(L, sizeof(sd_bus*));
	int err = shim_weak_stub(sd_bus_open)(bus);
	if (err < 0) return handle_error(L, -err);
	cache_pointer(L, BUS_CACHE_KEY, *bus); /* will be uncached (this is a new bus) */
	luaL_setmetatable(L, BUS_METATABLE);
	return 1;
}

static int bus_open_user(lua_State *L) {
	sd_bus **bus = lua_newuserdata(L, sizeof(sd_bus*));
	int err = shim_weak_stub(sd_bus_open_user)(bus);
	if (err < 0) return handle_error(L, -err);
	cache_pointer(L, BUS_CACHE_KEY, *bus); /* will be uncached (this is a new bus) */
	luaL_setmetatable(L, BUS_METATABLE);
	return 1;
}

static int bus_open_system(lua_State *L) {
	sd_bus **bus = lua_newuserdata(L, sizeof(sd_bus*));
	int err = shim_weak_stub(sd_bus_open_system)(bus);
	if (err < 0) return handle_error(L, -err);
	cache_pointer(L, BUS_CACHE_KEY, *bus); /* will be uncached (this is a new bus) */
	luaL_setmetatable(L, BUS_METATABLE);
	return 1;
}

static int bus_open_system_remote(lua_State *L) {
	const char *host = luaL_checkstring(L, 1);
	sd_bus **bus = lua_newuserdata(L, sizeof(sd_bus*));
	int err = shim_weak_stub(sd_bus_open_system_remote)(bus, host);
	if (err < 0) return handle_error(L, -err);
	cache_pointer(L, BUS_CACHE_KEY, *bus); /* will be uncached (this is a new bus) */
	luaL_setmetatable(L, BUS_METATABLE);
	return 1;
}

static int bus_open_system_machine(lua_State *L) {
	const char *machine = luaL_checkstring(L, 1);
	sd_bus **bus = lua_newuserdata(L, sizeof(sd_bus*));
	int err = shim_weak_stub(sd_bus_open_system_machine)(bus, machine);
	if (err < 0) return handle_error(L, -err);
	cache_pointer(L, BUS_CACHE_KEY, *bus); /* will be uncached (this is a new bus) */
	luaL_setmetatable(L, BUS_METATABLE);
	return 1;
}

static int bus_new(lua_State *L) {
	sd_bus **bus = lua_newuserdata(L, sizeof(sd_bus*));
	int err = shim_weak_stub(sd_bus_new)(bus);
	if (err < 0) return handle_error(L, -err);
	if (cache_pointer(L, BUS_CACHE_KEY, *bus)) {
		luaL_setmetatable(L, BUS_METATABLE);
	} else {
		shim_weak_stub(sd_bus_unref)(*bus);
	}
	return 1;
}

static int bus_unref(lua_State *L) {
	sd_bus **bus = luaL_checkudata(L, 1, BUS_METATABLE);
	if (*bus != NULL) {
		uncache_pointer(L, BUS_CACHE_KEY, *bus);
		shim_weak_stub(sd_bus_unref)(*bus);
		*bus = NULL;
	}
	return 0;
}

static int bus_set_address(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	const char *address = luaL_checkstring(L, 2);
	int err = shim_weak_stub(sd_bus_set_address)(bus, address);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int bus_get_address(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	const char *address;
	int err = shim_weak_stub(sd_bus_get_address)(bus, &address);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, address);
	return 1;
}

static int bus_set_bus_client(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	_Bool b = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	int err = shim_weak_stub(sd_bus_set_bus_client)(bus, b);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, 1);
	return 1;
}

static int bus_is_bus_client(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	int err = shim_weak_stub(sd_bus_is_bus_client)(bus);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int bus_set_server(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	_Bool b = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	/* if false, 3rd argument of server id is optional */
	sd_id128_t bus_id = ((!b && lua_isnoneornil(L, 3)) ? SD_ID128_NULL : check_id128_t(L, 3));
	int err = shim_weak_stub(sd_bus_set_server)(bus, b, bus_id);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, 1);
	return 1;
}

static int bus_is_server(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	int err = shim_weak_stub(sd_bus_is_server)(bus);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int bus_set_anonymous(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	_Bool b = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	int err = shim_weak_stub(sd_bus_set_anonymous)(bus, b);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, 1);
	return 1;
}

static int bus_is_anonymous(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	int err = shim_weak_stub(sd_bus_is_anonymous)(bus);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int bus_set_trusted(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	_Bool b = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	int err = shim_weak_stub(sd_bus_set_trusted)(bus, b);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, 1);
	return 1;
}

static int bus_is_trusted(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	int err = shim_weak_stub(sd_bus_is_trusted)(bus);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int bus_set_monitor(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	_Bool b = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	int err = shim_weak_stub(sd_bus_set_monitor)(bus, b);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, 1);
	return 1;
}

static int bus_is_monitor(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	int err = shim_weak_stub(sd_bus_is_monitor)(bus);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int bus_set_description(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	const char *description = luaL_optstring(L, 2, NULL);
	int err = shim_weak_stub(sd_bus_set_description)(bus, description);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int bus_get_description(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	const char *description;
	int err = shim_weak_stub(sd_bus_get_description)(bus, &description);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, description);
	return 1;
}

static int bus_negotiate_creds(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	_Bool b = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	uint64_t creds_mask = getcredsmask(L, 3);
	int err = shim_weak_stub(sd_bus_negotiate_creds)(bus, b, creds_mask);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, 1);
	return 1;
}

static int bus_negotiate_timestamp(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	_Bool b = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	int err = shim_weak_stub(sd_bus_negotiate_timestamp)(bus, b);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, 1);
	return 1;
}

static int bus_negotiate_fds(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	_Bool b = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	int err = shim_weak_stub(sd_bus_negotiate_fds)(bus, b);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, 1);
	return 1;
}

static int bus_start(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	int err = shim_weak_stub(sd_bus_start)(bus);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, 1);
	return 1;
}

static int bus_is_open(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	int err = shim_weak_stub(sd_bus_is_open)(bus);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int bus_get_bus_id(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	sd_id128_t *bus_id = lua_newuserdata(L, sizeof(sd_id128_t));
	int err = shim_weak_stub(sd_bus_get_bus_id)(bus, bus_id);
	if (err < 0) return handle_error(L, -err);
	luaL_setmetatable(L, ID128_METATABLE);
	return 1;
}

static int bus_get_scope(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	const char *scope;
	int err = shim_weak_stub(sd_bus_get_scope)(bus, &scope);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, scope);
	return 1;
}

static int bus_get_tid(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	pid_t tid;
	int err = shim_weak_stub(sd_bus_get_tid)(bus, &tid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, tid);
	return 1;
}

static int bus_creds_unref(lua_State *L) {
	sd_bus_creds **creds = luaL_checkudata(L, 1, BUS_CREDS_METATABLE);
	if (*creds != NULL) {
		uncache_pointer(L, BUS_CACHE_KEY, *creds);
		shim_weak_stub(sd_bus_creds_unref)(*creds);
		*creds = NULL;
	}
	return 0;
}

static int bus_creds_get_pid(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	pid_t pid;
	int err = shim_weak_stub(sd_bus_creds_get_pid)(creds, &pid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, pid);
	return 1;
}

static int bus_creds_get_ppid(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	pid_t ppid;
	int err = shim_weak_stub(sd_bus_creds_get_ppid)(creds, &ppid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, ppid);
	return 1;
}

static int bus_creds_get_tid(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	pid_t tid;
	int err = shim_weak_stub(sd_bus_creds_get_tid)(creds, &tid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, tid);
	return 1;
}

static int bus_creds_get_uid(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	uid_t uid;
	int err = shim_weak_stub(sd_bus_creds_get_uid)(creds, &uid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, uid);
	return 1;
}

static int bus_creds_get_euid(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	uid_t euid;
	int err = shim_weak_stub(sd_bus_creds_get_euid)(creds, &euid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, euid);
	return 1;
}

static int bus_creds_get_suid(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	uid_t suid;
	int err = shim_weak_stub(sd_bus_creds_get_suid)(creds, &suid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, suid);
	return 1;
}

static int bus_creds_get_fsuid(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	uid_t fsuid;
	int err = shim_weak_stub(sd_bus_creds_get_fsuid)(creds, &fsuid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, fsuid);
	return 1;
}

static int bus_creds_get_gid(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	gid_t gid;
	int err = shim_weak_stub(sd_bus_creds_get_gid)(creds, &gid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, gid);
	return 1;
}

static int bus_creds_get_egid(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	gid_t egid;
	int err = shim_weak_stub(sd_bus_creds_get_egid)(creds, &egid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, egid);
	return 1;
}

static int bus_creds_get_sgid(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	gid_t sgid;
	int err = shim_weak_stub(sd_bus_creds_get_sgid)(creds, &sgid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, sgid);
	return 1;
}

static int bus_creds_get_fsgid(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	gid_t fsgid;
	int err = shim_weak_stub(sd_bus_creds_get_fsgid)(creds, &fsgid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, fsgid);
	return 1;
}

static int bus_creds_get_supplementary_gids(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	const gid_t *gids;
	int i;
	int err = shim_weak_stub(sd_bus_creds_get_supplementary_gids)(creds, &gids);
	if (err < 0) return handle_error(L, -err);
	lua_newtable(L);
	for (i = 0; gids[i] != 0; i++) {
		lua_pushinteger(L, gids[i]);
		lua_rawseti(L, -2, i+1);
	}
	return 1;
}

static int bus_creds_get_comm(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	const char *comm;
	int err = shim_weak_stub(sd_bus_creds_get_comm)(creds, &comm);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, comm);
	return 1;
}

static int bus_creds_get_tid_comm(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	const char *comm;
	int err = shim_weak_stub(sd_bus_creds_get_tid_comm)(creds, &comm);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, comm);
	return 1;
}

static int bus_creds_get_exe(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	const char *exe;
	int err = shim_weak_stub(sd_bus_creds_get_exe)(creds, &exe);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, exe);
	return 1;
}

static int bus_creds_get_cmdline(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	char **cmdline;
	int i;
	int err = shim_weak_stub(sd_bus_creds_get_cmdline)(creds, &cmdline);
	if (err < 0) return handle_error(L, -err);
	lua_newtable(L);
	for (i = 0; cmdline[i] != NULL; i++) {
		lua_pushstring(L, cmdline[i]);
		lua_rawseti(L, -2, i); /* note: starts from 0 */
	}
	return 1;
}

static int bus_creds_get_cgroup(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	const char *cgroup;
	int err = shim_weak_stub(sd_bus_creds_get_cgroup)(creds, &cgroup);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, cgroup);
	return 1;
}

static int bus_creds_get_unit(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	const char *unit;
	int err = shim_weak_stub(sd_bus_creds_get_unit)(creds, &unit);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, unit);
	return 1;
}

static int bus_creds_get_slice(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	const char *slice;
	int err = shim_weak_stub(sd_bus_creds_get_slice)(creds, &slice);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, slice);
	return 1;
}

static int bus_creds_get_user_unit(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	const char *unit;
	int err = shim_weak_stub(sd_bus_creds_get_user_unit)(creds, &unit);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, unit);
	return 1;
}

static int bus_creds_get_user_slice(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	const char *slice;
	int err = shim_weak_stub(sd_bus_creds_get_user_slice)(creds, &slice);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, slice);
	return 1;
}

static int bus_creds_get_session(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	const char *session;
	int err = shim_weak_stub(sd_bus_creds_get_session)(creds, &session);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, session);
	return 1;
}

static int bus_creds_get_owner_uid(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	uid_t uid;
	int err = shim_weak_stub(sd_bus_creds_get_owner_uid)(creds, &uid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, uid);
	return 1;
}

static int bus_creds_has_effective_cap(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	int capability = luaL_checkinteger(L, 2);
	int err = shim_weak_stub(sd_bus_creds_has_effective_cap)(creds, capability);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int bus_creds_has_permitted_cap(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	int capability = luaL_checkinteger(L, 2);
	int err = shim_weak_stub(sd_bus_creds_has_permitted_cap)(creds, capability);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int bus_creds_has_inheritable_cap(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	int capability = luaL_checkinteger(L, 2);
	int err = shim_weak_stub(sd_bus_creds_has_inheritable_cap)(creds, capability);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int bus_creds_has_bounding_cap(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	int capability = luaL_checkinteger(L, 2);
	int err = shim_weak_stub(sd_bus_creds_has_bounding_cap)(creds, capability);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int bus_creds_get_selinux_context(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	const char *context;
	int err = shim_weak_stub(sd_bus_creds_get_selinux_context)(creds, &context);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, context);
	return 1;
}

static int bus_creds_get_audit_session_id(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	uint32_t sessionid;
	int err = shim_weak_stub(sd_bus_creds_get_audit_session_id)(creds, &sessionid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, sessionid);
	return 1;
}

static int bus_creds_get_audit_login_uid(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	uid_t loginuid;
	int err = shim_weak_stub(sd_bus_creds_get_audit_login_uid)(creds, &loginuid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, loginuid);
	return 1;
}

static int bus_creds_get_tty(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	const char *tty;
	int err = shim_weak_stub(sd_bus_creds_get_tty)(creds, &tty);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, tty);
	return 1;
}

static int bus_creds_get_unique_name(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	const char *name;
	int err = shim_weak_stub(sd_bus_creds_get_unique_name)(creds, &name);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, name);
	return 1;
}

static int bus_creds_get_well_known_names(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	char **names;
	int i;
	int err = shim_weak_stub(sd_bus_creds_get_well_known_names)(creds, &names);
	if (err < 0) return handle_error(L, -err);
	lua_newtable(L);
	for (i = 0; names[i] != NULL; i++) {
		lua_pushstring(L, names[i]);
		lua_rawseti(L, -2, i+1);
	}
	return 1;
}

static int bus_creds_get_description(lua_State *L) {
	sd_bus_creds *creds = check_bus_creds(L, 1);
	const char *name;
	int err = shim_weak_stub(sd_bus_creds_get_description)(creds, &name);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, name);
	return 1;
}

static int bus_get_owner_creds(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	uint64_t creds_mask = getcredsmask(L, 2);
	sd_bus_creds **ret = lua_newuserdata(L, sizeof(sd_bus_creds*));
	int err = shim_weak_stub(sd_bus_get_owner_creds)(bus, creds_mask, ret);
	if (err < 0) return handle_error(L, -err);
	/* docs not clear on whether this will be a new object
	 * inspection of the source suggests yes;
	 * but probably not guaranteed going forwards
	 */
	if (cache_pointer(L, BUS_CACHE_KEY, *ret)) {
		luaL_setmetatable(L, BUS_CREDS_METATABLE);
	} else {
		shim_weak_stub(sd_bus_creds_unref)(*ret);
	}
	return 1;
}

static int bus_send(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	sd_bus_message *message = check_bus_message(L, 2);
	_Bool want_cookie = (lua_isnoneornil(L, 3) ? 0 : (luaL_checktype(L, 3, LUA_TBOOLEAN), lua_toboolean(L, 3)));
	uint64_t cookie;
	int err = shim_weak_stub(sd_bus_send)(bus, message, want_cookie ? &cookie : NULL);
	if (err < 0) return handle_error(L, -err);
	if (want_cookie) {
		lua_pushuint64(L, cookie);
	} else {
		lua_pushboolean(L, 1);
	}
	return 1;
}

static int bus_send_to(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	sd_bus_message *message = check_bus_message(L, 2);
	const char *destination = luaL_checkstring(L, 3);
	_Bool want_cookie = (lua_isnoneornil(L, 4) ? 0 : (luaL_checktype(L, 4, LUA_TBOOLEAN), lua_toboolean(L, 4)));
	uint64_t cookie;
	int err = shim_weak_stub(sd_bus_send_to)(bus, message, destination, want_cookie ? &cookie : NULL);
	if (err < 0) return handle_error(L, -err);
	if (want_cookie) {
		lua_pushuint64(L, cookie);
	} else {
		lua_pushboolean(L, 1);
	}
	return 1;
}

/* returns nil, err, errno on commucation error
 * returns false, err, errno on dbus error
 * returns message on success
 */
static int bus_call(lua_State *L) {
	int err;
	sd_bus *bus = check_bus(L, 1);
	sd_bus_message *message = check_bus_message(L, 2);
	double timeout = luaL_optnumber(L, 3, 0); /* default is 0 */
	/* coerce math.huge to max uint value (which is infinite) */
	uint64_t timeout_usec = (timeout == HUGE_VAL)? ~0 : ceil(timeout * 1000000);
	sd_bus_error *error = lua_newuserdata(L, sizeof(sd_bus_error));
	sd_bus_message **reply;
	memset(error, 0, sizeof(sd_bus_error));
	luaL_setmetatable(L, BUS_ERROR_METATABLE);
	reply = lua_newuserdata(L, sizeof(sd_bus_message*));
	err = shim_weak_stub(sd_bus_call)(bus, message, timeout_usec, error, reply);
	if (err < 0) {
		if (shim_weak_stub(sd_bus_error_is_set)(error) > 0) {
			lua_pushboolean(L, 0);
			lua_pushvalue(L, -3);
			lua_pushinteger(L, shim_weak_stub(sd_bus_error_get_errno)(error));
			return 3;
		} else {
			return handle_error(L, -err);
		}
	}
	if (cache_pointer(L, BUS_CACHE_KEY, *reply)) {
		luaL_setmetatable(L, BUS_MESSAGE_METATABLE);
	} else {
		shim_weak_stub(sd_bus_message_unref)(*reply);
	}
	return 1;
}

static int bus_get_fd(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	int err = shim_weak_stub(sd_bus_get_fd)(bus);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, err);
	return 1;
}

static int bus_get_events(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	int err = shim_weak_stub(sd_bus_get_events)(bus);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, err);
	return 1;
}

static int bus_get_timeout(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	uint64_t timeout_usec;
	int err = shim_weak_stub(sd_bus_get_timeout)(bus, &timeout_usec);
	if (err < 0) return handle_error(L, -err);
	if (((uint64_t) -1) == timeout_usec) {
		lua_pushnumber(L, HUGE_VAL);
	} else {
		lua_pushnumber(L, ((double)timeout_usec)/1000000);
	}
	return 1;
}

static int bus_process(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	sd_bus_message **r = lua_newuserdata(L, sizeof(sd_bus_message*));
	int err = shim_weak_stub(sd_bus_process)(bus, r);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	if (*r == NULL) {
		return 1;
	} else {
		lua_pushvalue(L, -2);
		if (cache_pointer(L, BUS_CACHE_KEY, *r)) {
			luaL_setmetatable(L, BUS_MESSAGE_METATABLE);
		} else {
			shim_weak_stub(sd_bus_message_unref)(*r);
		}
		return 2;
	}
}

static int bus_process_priority(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	int64_t max_priority = luaL_checkinteger(L, 2);
	sd_bus_message **r = lua_newuserdata(L, sizeof(sd_bus_message*));
	int err = shim_weak_stub(sd_bus_process_priority)(bus, max_priority, r);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	if (*r == NULL) {
		return 1;
	} else {
		lua_pushvalue(L, -2);
		if (cache_pointer(L, BUS_CACHE_KEY, *r)) {
			luaL_setmetatable(L, BUS_MESSAGE_METATABLE);
		} else {
			shim_weak_stub(sd_bus_message_unref)(*r);
		}
		return 2;
	}
}

static int bus_wait(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	uint64_t timeout_usec;
	int err;
	if (lua_isnoneornil(L, 2)) { /* default to infinite wait */
		timeout_usec = -1;
	} else {
		timeout_usec = luaL_checknumber(L, 2) * 1000000;
	}
	err = shim_weak_stub(sd_bus_wait)(bus, timeout_usec);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, err);
	return 1;
}

static int bus_flush(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	int err = shim_weak_stub(sd_bus_flush)(bus);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, err);
	return 1;
}

static int bus_get_current_slot(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	sd_bus_slot **slot = lua_newuserdata(L, sizeof(sd_bus_slot*));
	*slot = shim_weak_stub(sd_bus_get_current_slot)(bus);
	if (NULL == *slot) {
		lua_pushnil(L);
	} else {
		if (cache_pointer(L, BUS_CACHE_KEY, *slot)) {
			shim_weak_stub(sd_bus_slot_ref)(*slot);
			luaL_setmetatable(L, BUS_SLOT_METATABLE);
		}
	}
	return 1;
}

static int bus_message_new_signal(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	const char *path = luaL_checkstring(L, 2);
	const char *interface = luaL_checkstring(L, 3);
	const char *member = luaL_checkstring(L, 4);
	sd_bus_message **message = lua_newuserdata(L, sizeof(sd_bus_message*));
	int err = shim_weak_stub(sd_bus_message_new_signal)(bus, message, path, interface, member);
	if (err < 0) return handle_error(L, -err);
	cache_pointer(L, BUS_CACHE_KEY, *message); /* will be uncached (this is a new message) */
	luaL_setmetatable(L, BUS_MESSAGE_METATABLE);
	return 1;
}

static int bus_message_new_method_call(lua_State *L) {
	sd_bus *bus = check_bus(L, 1);
	const char *destination = luaL_optstring(L, 2, NULL);
	const char *path = luaL_checkstring(L, 3);
	const char *interface = luaL_optstring(L, 4, NULL);
	const char *member = luaL_checkstring(L, 5);
	sd_bus_message **message = lua_newuserdata(L, sizeof(sd_bus_message*));
	int err = shim_weak_stub(sd_bus_message_new_method_call)(bus, message, destination, path, interface, member);
	if (err < 0) return handle_error(L, -err);
	cache_pointer(L, BUS_CACHE_KEY, *message); /* will be uncached (this is a new message) */
	luaL_setmetatable(L, BUS_MESSAGE_METATABLE);
	return 1;
}

static int bus_message_new_method_return(lua_State *L) {
	sd_bus_message *call = check_bus_message(L, 1);
	sd_bus_message **message = lua_newuserdata(L, sizeof(sd_bus_message*));
	int err = shim_weak_stub(sd_bus_message_new_method_return)(call, message);
	if (err < 0) return handle_error(L, -err);
	cache_pointer(L, BUS_CACHE_KEY, *message); /* will be uncached (this is a new message) */
	luaL_setmetatable(L, BUS_MESSAGE_METATABLE);
	return 1;
}

static int bus_message_new_method_error(lua_State *L) {
	sd_bus_message *call = check_bus_message(L, 1);
	sd_bus_error *e = check_bus_error(L, 2);
	sd_bus_message **message = lua_newuserdata(L, sizeof(sd_bus_message*));
	int err = shim_weak_stub(sd_bus_message_new_method_error)(call, message, e);
	if (err < 0) return handle_error(L, -err);
	cache_pointer(L, BUS_CACHE_KEY, *message); /* will be uncached (this is a new message) */
	luaL_setmetatable(L, BUS_MESSAGE_METATABLE);
	return 1;
}

static int bus_message_unref(lua_State *L) {
	sd_bus_message **message = luaL_checkudata(L, 1, BUS_MESSAGE_METATABLE);
	if (*message != NULL) {
		uncache_pointer(L, BUS_CACHE_KEY, *message);
		shim_weak_stub(sd_bus_message_unref)(*message);
		*message = NULL;
	}
	return 0;
}

static int bus_message_get_signature(lua_State *L) {
	sd_bus_message *m = check_bus_message(L, 1);
	int complete = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	const char *res = shim_weak_stub(sd_bus_message_get_signature)(m, complete);
	lua_pushstring(L, res);
	return 1;
}

static int bus_message_get_path(lua_State *L) {
	sd_bus_message *m = check_bus_message(L, 1);
	const char *res = shim_weak_stub(sd_bus_message_get_path)(m);
	lua_pushstring(L, res);
	return 1;
}

static int bus_message_get_interface(lua_State *L) {
	sd_bus_message *m = check_bus_message(L, 1);
	const char *res = shim_weak_stub(sd_bus_message_get_interface)(m);
	lua_pushstring(L, res);
	return 1;
}

static int bus_message_get_member(lua_State *L) {
	sd_bus_message *m = check_bus_message(L, 1);
	const char *res = shim_weak_stub(sd_bus_message_get_member)(m);
	lua_pushstring(L, res);
	return 1;
}

static int bus_message_get_destination(lua_State *L) {
	sd_bus_message *m = check_bus_message(L, 1);
	const char *res = shim_weak_stub(sd_bus_message_get_destination)(m);
	lua_pushstring(L, res);
	return 1;
}

static int bus_message_get_sender(lua_State *L) {
	sd_bus_message *m = check_bus_message(L, 1);
	const char *res = shim_weak_stub(sd_bus_message_get_sender)(m);
	lua_pushstring(L, res);
	return 1;
}

static int bus_message_get_bus(lua_State *L) {
	sd_bus_message *m = check_bus_message(L, 1);
	sd_bus **bus = lua_newuserdata(L, sizeof(sd_bus*));
	*bus = shim_weak_stub(sd_bus_message_get_bus)(m);
	if (*bus == NULL) {
		lua_pushnil(L);
	} else {
		/* the reference returned is not counted (yet) */
		if (cache_pointer(L, BUS_CACHE_KEY, *bus)) {
			shim_weak_stub(sd_bus_ref)(*bus);
			luaL_setmetatable(L, BUS_METATABLE);
		}
	}
	return 1;
}

static int bus_message_get_creds(lua_State *L) {
	sd_bus_message *m = check_bus_message(L, 1);
	sd_bus_creds **ret = lua_newuserdata(L, sizeof(sd_bus_creds*));
	*ret = shim_weak_stub(sd_bus_message_get_creds)(m);
	if (*ret == NULL) {
		lua_pushnil(L);
	} else {
		/* do not unref the result */
		if (cache_pointer(L, BUS_CACHE_KEY, *ret)) {
			shim_weak_stub(sd_bus_creds_ref)(*ret);
			luaL_setmetatable(L, BUS_CREDS_METATABLE);
		}
	}
	return 1;
}

static int bus_slot_unref(lua_State *L) {
	sd_bus_slot **slot = luaL_checkudata(L, 1, BUS_SLOT_METATABLE);
	if (*slot != NULL) {
		uncache_pointer(L, BUS_CACHE_KEY, *slot);
		shim_weak_stub(sd_bus_slot_unref)(*slot);
		*slot = NULL;
	}
	return 0;
}

static int bus_slot_get_bus(lua_State *L) {
	sd_bus_slot *m = check_bus_slot(L, 1);
	sd_bus **bus = lua_newuserdata(L, sizeof(sd_bus*));
	*bus = shim_weak_stub(sd_bus_slot_get_bus)(m);
	if (*bus == NULL) {
		lua_pushnil(L);
	} else {
		/* the reference returned is not counted (yet) */
		if (cache_pointer(L, BUS_CACHE_KEY, *bus)) {
			shim_weak_stub(sd_bus_ref)(*bus);
			luaL_setmetatable(L, BUS_METATABLE);
		}
	}
	return 1;
}

static int bus_new_error(lua_State *L) {
	sd_bus_error *error = lua_newuserdata(L, sizeof(sd_bus_error));
	memset(error, 0, sizeof(sd_bus_error));
	luaL_setmetatable(L, BUS_ERROR_METATABLE);
	return 1;
}

static int bus_error_free(lua_State *L) {
	sd_bus_error *e = check_bus_error(L, 1);
	shim_weak_stub(sd_bus_error_free)(e);
	return 0;
}

static int bus_error_get_name(lua_State *L) {
	sd_bus_error *e = check_bus_error(L, 1);
	lua_pushstring(L, e->name);
	return 1;
}

static int bus_error_get_message(lua_State *L) {
	sd_bus_error *e = check_bus_error(L, 1);
	lua_pushstring(L, e->message);
	return 1;
}

static int bus_error_is_set(lua_State *L) {
	sd_bus_error *e = check_bus_error(L, 1);
	_Bool res = shim_weak_stub(sd_bus_error_is_set)(e);
	lua_pushboolean(L, res);
	return 1;
}

static int bus_error_has_name(lua_State *L) {
	sd_bus_error *e = check_bus_error(L, 1);
	const char *name = luaL_checkstring(L, 2);
	_Bool res = shim_weak_stub(sd_bus_error_has_name)(e, name);
	lua_pushboolean(L, res);
	return 1;
}

static const luaL_Reg bus_lib[] = {
	{"default", bus_default},
	{"default_user", bus_default_user},
	{"default_system", bus_default_system},
	{"open", bus_open},
	{"open_user", bus_open_user},
	{"open_system", bus_open_system},
	{"open_system_remote", bus_open_system_remote},
	{"open_system_machine", bus_open_system_machine},
	{"new", bus_new},
	{"new_error", bus_new_error},
	{NULL, NULL}
};

static const luaL_Reg bus_methods[] = {
	{"set_address", bus_set_address},
	{"get_address", bus_get_address},
	{"set_bus_client", bus_set_bus_client},
	{"is_bus_client", bus_is_bus_client},
	{"set_server", bus_set_server},
	{"is_server", bus_is_server},
	{"set_anonymous", bus_set_anonymous},
	{"is_anonymous", bus_is_anonymous},
	{"set_trusted", bus_set_trusted},
	{"is_trusted", bus_is_trusted},
	{"set_monitor", bus_set_monitor},
	{"is_monitor", bus_is_monitor},
	{"set_description", bus_set_description},
	{"get_description", bus_get_description},
	{"negotiate_creds", bus_negotiate_creds},
	{"negotiate_timestamp", bus_negotiate_timestamp},
	{"negotiate_fds", bus_negotiate_fds},

	{"start", bus_start},

	{"send", bus_send},
	{"send_to", bus_send_to},
	{"call", bus_call},

	{"get_fd", bus_get_fd},
	{"get_events", bus_get_events},
	{"get_timeout", bus_get_timeout},
	{"process", bus_process},
	{"process_priority", bus_process_priority},
	{"wait", bus_wait},
	{"flush", bus_flush},

	{"get_current_slot", bus_get_current_slot},

	{"is_open", bus_is_open},

	{"get_bus_id", bus_get_bus_id},
	{"get_scope", bus_get_scope},
	{"get_tid", bus_get_tid},
	{"get_owner_creds", bus_get_owner_creds},

	{"message_new_signal", bus_message_new_signal},
	{"message_new_method_call", bus_message_new_method_call},

	{NULL, NULL}
};

static const luaL_Reg bus_creds_methods[] = {
	{"get_pid", bus_creds_get_pid},
	{"get_ppid", bus_creds_get_ppid},
	{"get_tid", bus_creds_get_tid},
	{"get_uid", bus_creds_get_uid},
	{"get_euid", bus_creds_get_euid},
	{"get_suid", bus_creds_get_suid},
	{"get_fsuid", bus_creds_get_fsuid},
	{"get_gid", bus_creds_get_gid},
	{"get_egid", bus_creds_get_egid},
	{"get_sgid", bus_creds_get_sgid},
	{"get_fsgid", bus_creds_get_fsgid},
	{"get_supplementary_gids", bus_creds_get_supplementary_gids},
	{"get_comm", bus_creds_get_comm},
	{"get_tid_comm", bus_creds_get_tid_comm},
	{"get_exe", bus_creds_get_exe},
	{"get_cmdline", bus_creds_get_cmdline},
	{"get_cgroup", bus_creds_get_cgroup},
	{"get_unit", bus_creds_get_unit},
	{"get_slice", bus_creds_get_slice},
	{"get_user_unit", bus_creds_get_user_unit},
	{"get_user_slice", bus_creds_get_user_slice},
	{"get_session", bus_creds_get_session},
	{"get_owner_uid", bus_creds_get_owner_uid},
	{"has_effective_cap", bus_creds_has_effective_cap},
	{"has_permitted_cap", bus_creds_has_permitted_cap},
	{"has_inheritable_cap", bus_creds_has_inheritable_cap},
	{"has_bounding_cap", bus_creds_has_bounding_cap},
	{"get_selinux_context", bus_creds_get_selinux_context},
	{"get_audit_session_id", bus_creds_get_audit_session_id},
	{"get_audit_login_uid", bus_creds_get_audit_login_uid},
	{"get_tty", bus_creds_get_tty},
	{"get_unique_name", bus_creds_get_unique_name},
	{"get_well_known_names", bus_creds_get_well_known_names},
	{"get_description", bus_creds_get_description},
	{NULL, NULL}
};

static const luaL_Reg bus_message_methods[] = {
	{"new_method_return", bus_message_new_method_return},
	{"new_method_error", bus_message_new_method_error},

	{"get_signature", bus_message_get_signature},
	{"get_path", bus_message_get_path},
	{"get_interface", bus_message_get_interface},
	{"get_member", bus_message_get_member},
	{"get_destination", bus_message_get_destination},
	{"get_sender", bus_message_get_sender},

	{"get_bus", bus_message_get_bus},
	{"get_creds", bus_message_get_creds},

	{NULL, NULL}
};

static const luaL_Reg bus_slot_methods[] = {
	{"get_bus", bus_slot_get_bus},

	{NULL, NULL}
};

static const luaL_Reg bus_error_methods[] = {
	{"get_name", bus_error_get_name},
	{"get_message", bus_error_get_message},

	{"is_set", bus_error_is_set},
	{"has_name", bus_error_has_name},
	{NULL, NULL}
};

int luaopen_systemd_bus_core (lua_State *L) {
	/* sd-bus API was introduced in systemd 221 */
	if (!symbol_exists("sd_bus_open")) {
		return luaL_error(L, "sd-bus unavailable");
	}

	/* ensure ID128_METATABLE is loaded */
	luaL_requiref(L, "systemd.id128.core", luaopen_systemd_id128_core, 0);

	luaL_newlib(L, bus_lib);

	/* create weak table in registry */
	lua_newtable(L);
	lua_newtable(L);
	lua_pushliteral(L, "v");
	lua_setfield(L, -2, "__mode");
	lua_setmetatable(L, -2);
	lua_setfield(L, LUA_REGISTRYINDEX, BUS_CACHE_KEY);

	if (luaL_newmetatable(L, BUS_METATABLE) != 0) {
		luaL_newlib(L, bus_methods);
		lua_setfield(L, -2, "__index");
		lua_pushcfunction(L, bus_unref);
		lua_setfield(L, -2, "__gc");
		lua_pushcfunction(L, tostring);
		lua_setfield(L, -2, "__tostring");
	}
	/* Expose bus methods */
	lua_getfield(L, -1, "__index");
	lua_setfield(L, -3, "BUS_METHODS");
	lua_pop(L, 1);

	if (luaL_newmetatable(L, BUS_CREDS_METATABLE) != 0) {
		luaL_newlib(L, bus_creds_methods);
		lua_setfield(L, -2, "__index");
		lua_pushcfunction(L, bus_creds_unref);
		lua_setfield(L, -2, "__gc");
		lua_pushcfunction(L, tostring);
		lua_setfield(L, -2, "__tostring");
	}
	/* Expose bus creds methods */
	lua_getfield(L, -1, "__index");
	lua_setfield(L, -3, "BUS_CREDS_METHODS");
	lua_pop(L, 1);

	if (luaL_newmetatable(L, BUS_MESSAGE_METATABLE) != 0) {
		luaL_newlib(L, bus_message_methods);
		lua_setfield(L, -2, "__index");
		lua_pushcfunction(L, bus_message_unref);
		lua_setfield(L, -2, "__gc");
		lua_pushcfunction(L, tostring);
		lua_setfield(L, -2, "__tostring");
	}
	/* Expose bus message methods */
	lua_getfield(L, -1, "__index");
	lua_setfield(L, -3, "BUS_MESSAGE_METHODS");
	lua_pop(L, 1);

	if (luaL_newmetatable(L, BUS_SLOT_METATABLE) != 0) {
		luaL_newlib(L, bus_slot_methods);
		lua_setfield(L, -2, "__index");
		lua_pushcfunction(L, bus_slot_unref);
		lua_setfield(L, -2, "__gc");
		lua_pushcfunction(L, tostring);
		lua_setfield(L, -2, "__tostring");
	}
	/* Expose bus slot methods */
	lua_getfield(L, -1, "__index");
	lua_setfield(L, -3, "BUS_SLOT_METHODS");
	lua_pop(L, 1);

	if (luaL_newmetatable(L, BUS_ERROR_METATABLE) != 0) {
		luaL_newlib(L, bus_error_methods);
		lua_setfield(L, -2, "__index");
		lua_pushcfunction(L, bus_error_free);
		lua_setfield(L, -2, "__gc");
		lua_pushcfunction(L, tostring);
		lua_setfield(L, -2, "__tostring");
	}
	/* Expose bus error methods */
	lua_getfield(L, -1, "__index");
	lua_setfield(L, -3, "BUS_ERROR_METHODS");
	lua_setfield(L, -2, "BUS_ERROR_METATABLE");

	return 1;
}
