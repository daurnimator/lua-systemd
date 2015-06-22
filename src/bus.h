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

int luaopen_systemd_bus_core (lua_State *L);
