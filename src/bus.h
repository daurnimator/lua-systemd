#include "lua.h"
#include "lauxlib.h"

#include <systemd/sd-bus.h>

#define BUS_METATABLE "sd_bus*"
#define BUS_CREDS_METATABLE "sd_bus_creds*"
#define BUS_MESSAGE_METATABLE "sd_bus_message*"
#define BUS_ERROR_METATABLE "sd_bus_error*"

static sd_bus* check_bus(lua_State *L, int index) {
	sd_bus **bus = luaL_checkudata(L, index, BUS_METATABLE);
	if (*bus == NULL) luaL_error(L, "Invalid bus handle");
	return *bus;
}

static sd_bus_creds* check_bus_creds(lua_State *L, int index) {
	sd_bus_creds **creds = luaL_checkudata(L, index, BUS_CREDS_METATABLE);
	if (*creds == NULL) luaL_error(L, "Invalid bus credentials handle");
	return *creds;
}

static sd_bus_message* check_bus_message(lua_State *L, int index) {
	sd_bus_message **message = luaL_checkudata(L, index, BUS_MESSAGE_METATABLE);
	if (*message == NULL) luaL_error(L, "Invalid bus message handle");
	return *message;
}

static sd_bus_error* check_bus_error(lua_State *L, int index) {
	sd_bus_error *error = luaL_checkudata(L, index, BUS_ERROR_METATABLE);
	return error;
}

static _Bool getflag(lua_State *L, int idx, const char *key) {
	_Bool r;
	lua_getfield(L, idx, key);
	r = lua_toboolean(L, -1);
	lua_pop(L, 1);
	return r;
}

static uint64_t getcredsmask(lua_State *L, int idx) {
	uint64_t creds_mask;
	switch(lua_type(L, idx)) {
	case LUA_TTABLE:
		creds_mask = 0;
		if (getflag(L, idx, "pid")) creds_mask |= SD_BUS_CREDS_PID;
		if (getflag(L, idx, "tid")) creds_mask |= SD_BUS_CREDS_TID;
		if (getflag(L, idx, "ppid")) creds_mask |= SD_BUS_CREDS_PPID;
		if (getflag(L, idx, "uid")) creds_mask |= SD_BUS_CREDS_UID;
		if (getflag(L, idx, "euid")) creds_mask |= SD_BUS_CREDS_EUID;
		if (getflag(L, idx, "suid")) creds_mask |= SD_BUS_CREDS_SUID;
		if (getflag(L, idx, "fsuid")) creds_mask |= SD_BUS_CREDS_FSUID;
		if (getflag(L, idx, "gid")) creds_mask |= SD_BUS_CREDS_GID;
		if (getflag(L, idx, "egid")) creds_mask |= SD_BUS_CREDS_EGID;
		if (getflag(L, idx, "sgid")) creds_mask |= SD_BUS_CREDS_SGID;
		if (getflag(L, idx, "fsgid")) creds_mask |= SD_BUS_CREDS_FSGID;
		if (getflag(L, idx, "supplementary_gids")) creds_mask |= SD_BUS_CREDS_SUPPLEMENTARY_GIDS;
		if (getflag(L, idx, "comm")) creds_mask |= SD_BUS_CREDS_COMM;
		if (getflag(L, idx, "tid_comm")) creds_mask |= SD_BUS_CREDS_TID_COMM;
		if (getflag(L, idx, "exe")) creds_mask |= SD_BUS_CREDS_EXE;
		if (getflag(L, idx, "cmdline")) creds_mask |= SD_BUS_CREDS_CMDLINE;
		if (getflag(L, idx, "cgroup")) creds_mask |= SD_BUS_CREDS_CGROUP;
		if (getflag(L, idx, "unit")) creds_mask |= SD_BUS_CREDS_UNIT;
		if (getflag(L, idx, "slice")) creds_mask |= SD_BUS_CREDS_SLICE;
		if (getflag(L, idx, "user_unit")) creds_mask |= SD_BUS_CREDS_USER_UNIT;
		if (getflag(L, idx, "user_slice")) creds_mask |= SD_BUS_CREDS_USER_SLICE;
		if (getflag(L, idx, "session")) creds_mask |= SD_BUS_CREDS_SESSION;
		if (getflag(L, idx, "owner_uid")) creds_mask |= SD_BUS_CREDS_OWNER_UID;
		if (getflag(L, idx, "effective_caps")) creds_mask |= SD_BUS_CREDS_EFFECTIVE_CAPS;
		if (getflag(L, idx, "permitted_caps")) creds_mask |= SD_BUS_CREDS_PERMITTED_CAPS;
		if (getflag(L, idx, "inheritable_caps")) creds_mask |= SD_BUS_CREDS_INHERITABLE_CAPS;
		if (getflag(L, idx, "bounding_caps")) creds_mask |= SD_BUS_CREDS_BOUNDING_CAPS;
		if (getflag(L, idx, "selinux_context")) creds_mask |= SD_BUS_CREDS_SELINUX_CONTEXT;
		if (getflag(L, idx, "audit_session_id")) creds_mask |= SD_BUS_CREDS_AUDIT_SESSION_ID;
		if (getflag(L, idx, "audit_login_uid")) creds_mask |= SD_BUS_CREDS_AUDIT_LOGIN_UID;
		if (getflag(L, idx, "tty")) creds_mask |= SD_BUS_CREDS_TTY;
		if (getflag(L, idx, "unique_name")) creds_mask |= SD_BUS_CREDS_UNIQUE_NAME;
		if (getflag(L, idx, "well_known_names")) creds_mask |= SD_BUS_CREDS_WELL_KNOWN_NAMES;
		if (getflag(L, idx, "description")) creds_mask |= SD_BUS_CREDS_DESCRIPTION;
		if (getflag(L, idx, "augment")) creds_mask |= SD_BUS_CREDS_AUGMENT;
		break;
	case LUA_TBOOLEAN:
		if (lua_toboolean(L, idx)) {
			creds_mask = _SD_BUS_CREDS_ALL | SD_BUS_CREDS_AUGMENT;
			break;
		}
		/* fall-through */
	default:
		luaL_argerror(L, idx, "expected table of flags");
		/* unreachable */
	}
	return creds_mask;
}

int luaopen_systemd_bus_core (lua_State *L);
