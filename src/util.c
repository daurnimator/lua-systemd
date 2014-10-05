#include <string.h>

static int handle_error(lua_State *L, int err) {
    lua_pushnil(L);
    lua_pushstring(L, strerror(err));
    lua_pushinteger(L, err);
    return 3;
}
