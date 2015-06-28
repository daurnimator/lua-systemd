package = "systemd"
version = "scm-0"
source = {
	url = "git://github.com/daurnimator/lua-systemd"
}
description = {
	summary = "Lua bindings to systemd",
	homepage = "https://github.com/daurnimator/lua-systemd",
	license = "MIT/X11"
}
dependencies = {
	"lua >= 5.0, < 5.4"
}
build = {
	type = "builtin",
	modules = {
		["systemd.init"] = "src/init.lua",
		["systemd.daemon"] = "src/daemon.lua",
		["systemd"] = {
			sources = {
				"src/daemon.c",
				"src/id128.c",
				"src/messages.c",
				"src/journal.c",
				"src/login.c",
				"vendor/compat-5.3/c-api/compat-5.3.c",
			},
			defines = { "_GNU_SOURCE" }, -- for RTLD_DEFAULT
			libraries = { "systemd" },
			incdirs = { "vendor/compat-5.3/c-api/" }
		},
		["systemd.id128"] = "src/id128.lua",
		["systemd.journal"] = "src/journal.lua",
		["systemd.login"] = "src/login.lua",
		["systemd.messages"] = "src/messages.lua"
	}
}
