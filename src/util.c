#include "lua.h"
#include "lauxlib.h" /* luaL_checknumber */
#include "compat-5.3.h"

#include <string.h> /* strerror */
#include <dlfcn.h> /* dlsym, dlerror */


static int handle_error(lua_State *L, int err) {
	lua_pushnil(L);
	lua_pushstring(L, strerror(err));
	lua_pushinteger(L, err);
	return 3;
}

/* examines the object on top of the stack
   if the passed pointer is already in the table at the given key in the registry
     push the value associated with it onto the stack (replacing current value on the stack) and returns false
   otherwise, adds it to that table at the given key (and leaves the object on the stack) and returns true
 */

static _Bool cache_pointer(lua_State *L, void *key, void* p) {
	lua_rawgetp(L, LUA_REGISTRYINDEX, key);
	if (LUA_TNIL == lua_rawgetp(L, -1, p)) {
		lua_pushvalue(L, -3);
		lua_rawsetp(L, -3, p);
		lua_pop(L, 2); /* pop table + nil */
		return 1;
	} else { /* stack: original value, table, new value */
		lua_insert(L, lua_absindex(L, -3));
		lua_pop(L, 2); /* pop original value + table */
		return 0;
	}
}

static void uncache_pointer(lua_State *L, void *key, void* p) {
	lua_rawgetp(L, LUA_REGISTRYINDEX, key);
	lua_pushnil(L);
	lua_rawsetp(L, -2, p);
	lua_pop(L, 1);
}


#ifndef lua_pushuint64
#define lua_pushuint64 lua_pushnumber
#endif

#ifndef luaL_checkuint64
#define luaL_checkuint64 luaL_checknumber
#endif

/* This hack is required as lua always passes RTLD_NOW to dlopen
 * Without this loading lua-systemd would fail with an error about undefined symbols
 * e.g. sd_machine_get_ifindices missing on a system running systemd 213
 *
 * To ensure graceful fallback we have to have some indirection via a function pointer.
*/
#define shim_weak_stub_name(symbol) symbol ## _stub
#define shim_weak_stub(symbol) symbol ## _pointer
#define shim_weak_stub_declare(ret_type, symbol, args, val) \
	static ret_type (*shim_weak_stub(symbol)) args; \
	static ret_type shim_weak_stub_name(symbol) args { return val; } \
	__attribute__((constructor)) static void initialize_ ## symbol() { \
		if ((shim_weak_stub(symbol) = dlsym(RTLD_DEFAULT, #symbol)) == NULL) { \
			shim_weak_stub(symbol) = shim_weak_stub_name(symbol); \
		} \
	}

#define symbol_exists(name) (dlsym(RTLD_DEFAULT, name) != NULL || dlerror() == NULL)
#define set_func(L, func, name) (lua_pushcclosure(L, func, 0), lua_setfield(L, -2, name))
#define set_func_if_symbol_exists(symbol, L, func, name) if (symbol_exists(symbol)) set_func(L, func, name)
