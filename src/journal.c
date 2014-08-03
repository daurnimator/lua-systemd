#include "lua.h"
#include "lauxlib.h"
#include "compat-5.2.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define SD_JOURNAL_SUPPRESS_LOCATION
#include <systemd/sd-journal.h>

static int handle_log_result (lua_State *L, int err) {
	if (err == 0) {
		lua_pushboolean(L, 1);
		return 1;
	} else {
		lua_pushnil(L);
		lua_pushstring(L, strerror(-err));
		lua_pushinteger(L, -err);
		return 3;
	}
}

static int sendv (lua_State *L) {
	int i, res;
	size_t n;
	struct iovec *iov;
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_settop(L, 1);
	n = lua_rawlen(L, 1);
	if ((iov = calloc(n, sizeof(struct iovec))) == NULL) return luaL_error(L, "out of memory");
	for (i=0; i<n; i++) {
		lua_rawgeti(L, -1, i+1);
		if (lua_type(L, -1) != LUA_TSTRING) {
			free(iov);
			return luaL_argerror(L, 1, "non-string table entry");
		}
		iov[i].iov_base = (void*)lua_tolstring(L, -1, &iov[i].iov_len);
		lua_pop(L, 1);
	}
	res = sd_journal_sendv(iov, n);
	free(iov);
	return handle_log_result(L, res);
}

static int _perror (lua_State *L) {
	const char *message = luaL_checkstring(L, 1);
	return handle_log_result(L, sd_journal_perror(message));
}

static int io_fclose (lua_State *L) {
#if !defined(LUA_VERSION_NUM) || LUA_VERSION_NUM == 501
/* Lua 5.1 doesn't have an easy way to make your own file objects */
#ifndef LUA_FILEHANDLE
#define LUA_FILEHANDLE "FILE*"
#endif
/* From http://www.lua.org/source/5.1/liolib.c.html#io_fclose */
	FILE **p = (FILE **)luaL_checkudata(L, 1, LUA_FILEHANDLE);
	int ok = (fclose(*p) == 0);
	int en;
	if (ok) {
		*p = NULL;
		lua_pushboolean(L, 1);
		return 1;
	} else {
		en = errno;  /* calls to Lua API may change this value */
		lua_pushnil(L);
		lua_pushstring(L, strerror(en));
		lua_pushinteger(L, en);
		return 3;
	}
#else
/* From http://www.lua.org/source/5.2/liolib.c.html#io_fclose */
	luaL_Stream *p = (luaL_Stream *)luaL_checkudata(L, 1, LUA_FILEHANDLE);
	int res = fclose(p->f);
	return luaL_fileresult(L, (res == 0), NULL);
#endif
}

static int stream_fd (lua_State *L) {
	int fd;
	const char *identifier = luaL_checkstring(L, 1);
	int priority = luaL_checkint(L, 2);
	int level_prefix = lua_toboolean(L, 3); /* Optional arg, defaults to false */
	#if !defined(LUA_VERSION_NUM) || LUA_VERSION_NUM == 501
	FILE **pf = (FILE **)lua_newuserdata(L, sizeof(FILE *));
	*pf = NULL;
	luaL_setmetatable(L, LUA_FILEHANDLE);
	if ((fd = sd_journal_stream_fd(identifier, priority, level_prefix)) < 0) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(-fd));
		lua_pushinteger(L, -fd);
		return 3;
	}
	luaL_getmetatable(L, LUA_FILEHANDLE);
	lua_setmetatable(L, -2);
	*pf = fdopen(fd, "w");
	#else
	luaL_Stream *p = (luaL_Stream *)lua_newuserdata(L, sizeof(luaL_Stream));
	p->closef = NULL; /* create a `closed' file handle before opening file, in case of errors */
	luaL_setmetatable(L, LUA_FILEHANDLE);
	if ((fd = sd_journal_stream_fd(identifier, priority, level_prefix)) < 0) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(-fd));
		lua_pushinteger(L, -fd);
		return 3;
	}
	p->f = fdopen(fd, "w");
	p->closef = &io_fclose;
	#endif
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
