#include "lua.h"
#include "lauxlib.h"
#include "compat-5.2.h"

#include <stdlib.h>

#define SD_JOURNAL_SUPPRESS_LOCATION
#include <systemd/sd-journal.h>

#include "util.c"

#define JOURNAL_METATABLE "sd_journal"


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

static int journal_open (lua_State *L) {
	int err;
	int flags = luaL_optint(L, 1, 0);
	sd_journal **j = lua_newuserdata(L, sizeof(sd_journal*));
	err = sd_journal_open(j, flags);
	if (err != 0) return handle_error(L, -err);
	luaL_setmetatable(L, JOURNAL_METATABLE);
	return 1;
}

static int journal_open_directory (lua_State *L) {
	int err;
	const char *path = luaL_checkstring(L, 1);
	int flags = luaL_optint(L, 2, 0);
	sd_journal **j = lua_newuserdata(L, sizeof(sd_journal*));
	err = sd_journal_open_directory(j, path, flags);
	if (err != 0) return handle_error(L, -err);
	luaL_setmetatable(L, JOURNAL_METATABLE);
	return 1;
}

static int journal_open_files (lua_State *L) {
	int err;
	sd_journal **j;
	const char **paths;
	size_t len;
	int flags;
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_settop(L, 2);
	len = lua_rawlen(L, 1);
	paths = lua_newuserdata(L, sizeof(const char*)*(len+1));
	paths[len] = NULL;
	for (; len>0; len--) {
		lua_rawgeti(L, 1, len);
		paths[len-1] = luaL_checkstring(L, -1);
		lua_pop(L, 1);
	}
	flags = luaL_optint(L, 2, 0);
	j = lua_newuserdata(L, sizeof(sd_journal*));
	err = sd_journal_open_files(j, paths, flags);
	if (err != 0) return handle_error(L, -err);
	luaL_setmetatable(L, JOURNAL_METATABLE);
	return 1;
}

static int journal_open_container (lua_State *L) {
	int err;
	const char *machine = luaL_checkstring(L, 1);
	int flags = luaL_optint(L, 2, 0);
	sd_journal **j = lua_newuserdata(L, sizeof(sd_journal*));
	err = sd_journal_open_container(j, machine, flags);
	if (err != 0) return handle_error(L, -err);
	luaL_setmetatable(L, JOURNAL_METATABLE);
	return 1;
}

static int journal_close (lua_State *L) {
	sd_journal **jp = luaL_checkudata(L, 1, JOURNAL_METATABLE);
	if (*jp != NULL) {
		sd_journal_close(*jp);
		*jp = NULL;
	}
	return 0;
}

static sd_journal* check_journal(lua_State *L, int index) {
	sd_journal **jp = luaL_checkudata(L, index, JOURNAL_METATABLE);
	if (*jp == NULL) luaL_error(L, "Invalid journal handle");
	return *jp;
}

static int journal_next (lua_State *L) {
	sd_journal *j = check_journal(L, 1);
	int err = sd_journal_next(j);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, err);
	return 1;
}

static int journal_next_skip (lua_State *L) {
	sd_journal *j = check_journal(L, 1);
	uint64_t skip = luaL_checkinteger(L, 2);
	int err = sd_journal_next_skip(j, skip);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, err);
	return 1;
}

static int journal_previous (lua_State *L) {
	sd_journal *j = check_journal(L, 1);
	int err = sd_journal_previous(j);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, err);
	return 1;
}

static int journal_previous_skip (lua_State *L) {
	sd_journal *j = check_journal(L, 1);
	uint64_t skip = luaL_checkinteger(L, 2);
	int err = sd_journal_previous_skip(j, skip);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, err);
	return 1;
}

static const luaL_Reg journal_methods[] = {
	{"next", journal_next},
	{"next_skip", journal_next_skip},
	{"previous", journal_previous},
	{"previous_skip", journal_previous_skip},
	{NULL, NULL}
};

int luaopen_systemd_journal_core (lua_State *L) {
	static const luaL_Reg lib[] = {
		{"sendv", sendv},
		{"perror", _perror},
		{"stream_fd", stream_fd},
		{"open", journal_open},
		{"open_directory", journal_open_directory},
		{"open_files", journal_open_files},
		{"open_container", journal_open_container},
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

	if (luaL_newmetatable(L, JOURNAL_METATABLE) != 0) {
		lua_pushcfunction(L, journal_close);
		lua_setfield(L, -2, "__gc");
		luaL_newlib(L, journal_methods);
		lua_setfield(L, -2, "__index");
	}
	lua_pop(L, 1);

	lua_createtable(L, 0, 4);
	lua_pushnumber(L, SD_JOURNAL_LOCAL_ONLY); lua_setfield(L, -2, "LOCAL_ONLY");
	lua_pushnumber(L, SD_JOURNAL_RUNTIME_ONLY); lua_setfield(L, -2, "RUNTIME_ONLY");
	lua_pushnumber(L, SD_JOURNAL_SYSTEM); lua_setfield(L, -2, "SYSTEM");
	lua_pushnumber(L, SD_JOURNAL_CURRENT_USER); lua_setfield(L, -2, "CURRENT_USER");
	lua_setfield(L, -2, "OPEN");

	return 1;
}
