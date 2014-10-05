#include "lua.h"
#include "lauxlib.h"
#include "compat-5.2.h"

#include <stdlib.h>

#define SD_JOURNAL_SUPPRESS_LOCATION
#include <systemd/sd-journal.h>

#include "util.c"


static int handle_log_result (lua_State *L, int err) {
	if (err == 0) {
		lua_pushboolean(L, 1);
		return 1;
	} else {
		return handle_error(L, -err);
	}
}

static int sendv (lua_State *L) {
	int i, res;
	size_t n;
	struct iovec *iov;
	luaL_checktype(L, 1, LUA_TTABLE);
	n = lua_rawlen(L, 1);
	iov = lua_newuserdata(L, n*sizeof(struct iovec)); /* allocate via lua so we get free cleanup */
	for (i=0; i<n; i++) {
		lua_rawgeti(L, 1, i+1);
		/* Make sure value is a string, we do **not** want automatic coercion */
		if (lua_type(L, -1) != LUA_TSTRING) {
			return luaL_argerror(L, 1, "non-string table entry");
		}
		iov[i].iov_base = (void*)lua_tolstring(L, -1, &iov[i].iov_len);
		lua_pop(L, 1);
	}
	res = sd_journal_sendv(iov, n);
	return handle_log_result(L, res);
}

static int _perror (lua_State *L) {
	const char *message = luaL_checkstring(L, 1);
	return handle_log_result(L, sd_journal_perror(message));
}

/* From http://www.lua.org/source/5.2/liolib.c.html#io_fclose */
static int io_fclose (lua_State *L) {
	luaL_Stream *p = (luaL_Stream *)luaL_checkudata(L, 1, LUA_FILEHANDLE);
	FILE *pf = p->f;
	int res = fclose(pf);
	return luaL_fileresult(L, (res == 0), NULL);
}

static int stream_fd (lua_State *L) {
	int fd;
	const char *identifier = luaL_checkstring(L, 1);
	int priority = luaL_checkint(L, 2);
	int level_prefix = lua_toboolean(L, 3); /* Optional arg, defaults to false */
	luaL_Stream *p = (luaL_Stream *)lua_newuserdata(L, sizeof(luaL_Stream));
	p->closef = NULL; /* create a `closed' file handle before opening file, in case of errors */
	luaL_setmetatable(L, LUA_FILEHANDLE);
	fd = sd_journal_stream_fd(identifier, priority, level_prefix);
	if (fd < 0) return handle_error(L, -fd);
	p->f = fdopen(fd, "w");
	p->closef = &io_fclose;
	return 1;
}

int luaopen_systemd_journal_core (lua_State *L) {
	static const luaL_Reg lib[] = {
		{"sendv", sendv},
		{"perror", _perror},
		{"stream_fd", stream_fd},
		{NULL, NULL}
	};
	luaL_newlib(L, lib);

	/* Even with compat-5.2, Lua 5.1 doesn't have an easy way to make your own file objects */
	/* Set up function environment for stream_fd for 5.1 so handle gets closed correctly */
	#if !defined(LUA_VERSION_NUM) || LUA_VERSION_NUM == 501
	lua_getfield(L, -1, "stream_fd");
	lua_createtable(L, 0, 1);
	lua_pushcfunction(L, &io_fclose);
	lua_setfield(L, -2, "__close");
	lua_setfenv(L, -2);
	lua_pop(L, 1);
	#endif

	lua_createtable(L, 0, 3);
	lua_pushnumber(L, SD_JOURNAL_NOP); lua_setfield(L, -2, "NOP");
	lua_pushnumber(L, SD_JOURNAL_APPEND); lua_setfield(L, -2, "APPEND");
	lua_pushnumber(L, SD_JOURNAL_INVALIDATE); lua_setfield(L, -2, "INVALIDATE");
	lua_setfield(L, -2, "WAKEUP");

	return 1;
}
