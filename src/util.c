#include "lua.h"

#include <string.h> /* strerror */
#include <dlfcn.h> /* dlopen, dlsym */
#include <errno.h> /* ENOTSUP */

static int handle_error(lua_State *L, int err) {
	lua_pushnil(L);
	lua_pushstring(L, strerror(err));
	lua_pushinteger(L, err);
	return 3;
}

#ifndef lua_pushuint64
#define lua_pushuint64 lua_pushnumber
#endif

#ifndef luaL_checkuint64
#define luaL_checkuint64 luaL_checknumber
#endif

#define weak_define(ret_type, symbol, args, val) ret_type __attribute__((weak)) symbol args { return val; }
#define weak_ENOTSUP_define(symbol, ...) weak_define(int, symbol, (__VA_ARGS__), -ENOTSUP)

static int symbol_exists(const char *path, const char *name) {
	void *handle = dlopen(path, RTLD_LAZY|RTLD_LOCAL);
	if (dlerror() != NULL) return 0;
	void *sym = dlsym(handle, name);
	return dlerror() == NULL;
}

#define set_func(L, func, name) (lua_pushcclosure(L, func, 0), lua_setfield(L, -2, name))

#define systemd_has(name) symbol_exists("libsystemd.so", "LIBSYSTEMD_" #name)
