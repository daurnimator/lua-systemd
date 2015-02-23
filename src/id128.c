#include "lua.h"
#include "lauxlib.h"
#include "compat-5.3.h"

#include <systemd/sd-id128.h>

#include "util.c"
#include "id128.h"
#include "messages.h"

static int randomize (lua_State *L) {
	sd_id128_t *ret = lua_newuserdata(L, sizeof(sd_id128_t));
	int err = sd_id128_randomize(ret);
	if (err < 0) return handle_error(L, -err);
	luaL_setmetatable(L, ID128_METATABLE);
	return 1;
}

static int from_string (lua_State *L) {
	size_t l;
	const char *s = luaL_checklstring(L, 1, &l);
	if (l != 32 && l != 37) return luaL_argerror(L, 1, "string must be 32 hex characters or a 37 character formatted RFC UUID");
	sd_id128_t *ret = lua_newuserdata(L, sizeof(sd_id128_t));
	int err = sd_id128_from_string(s, ret);
	if (err < 0) return handle_error(L, -err);
	luaL_setmetatable(L, ID128_METATABLE);
	return 1;
}

static int get_machine (lua_State *L) {
	sd_id128_t *ret = lua_newuserdata(L, sizeof(sd_id128_t));
	int err = sd_id128_get_machine(ret);
	if (err < 0) return handle_error(L, -err);
	luaL_setmetatable(L, ID128_METATABLE);
	return 1;
}

static int get_boot (lua_State *L) {
	sd_id128_t *ret = lua_newuserdata(L, sizeof(sd_id128_t));
	int err = sd_id128_get_boot(ret);
	if (err < 0) return handle_error(L, -err);
	luaL_setmetatable(L, ID128_METATABLE);
	return 1;
}

static int equal (lua_State *L) {
	sd_id128_t *a = (sd_id128_t*)luaL_checkudata(L, 1, ID128_METATABLE);
	sd_id128_t *b = (sd_id128_t*)luaL_checkudata(L, 2, ID128_METATABLE);
	lua_pushboolean(L, sd_id128_equal(*a, *b));
	return 1;
}

static int to_string (lua_State *L) {
	sd_id128_t *a = (sd_id128_t*)luaL_checkudata(L, 1, ID128_METATABLE);
	char s[33];
	sd_id128_to_string(*a, s);
	lua_pushlstring(L, s, 32);
	return 1;
}

int luaopen_systemd_id128_core (lua_State *L) {
	static const luaL_Reg lib[] = {
		{"randomize", randomize},
		{"from_string", from_string},
		{"get_machine", get_machine},
		{"get_boot", get_boot},
		{NULL, NULL}
	};
	luaL_newlib(L, lib);

	static const luaL_Reg meta[] = {
		{"__eq", equal},
		{"__tostring", to_string},
		{NULL, NULL}
	};

	static const luaL_Reg methods[] = {
		{"to_string", to_string},
		{"get_catalog", journal_get_catalog_for_message_id},
		{NULL, NULL}
	};

	if (luaL_newmetatable(L, ID128_METATABLE)) {
		luaL_newlib(L, methods);
		lua_setfield(L, -2, "__index");
		luaL_setfuncs(L, meta, 0);
	}
	/* Expose id128 methods */
	lua_getfield(L, -1, "__index");
	lua_setfield(L, -3, "ID128_METHODS");

	lua_pop(L, 1);

	return 1;
}
