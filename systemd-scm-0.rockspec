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
   "lua >= 5.0, < 5.3"
}
build = {
   type = "builtin",
   modules = {
      ["systemd"] = "src/init.lua",
      ["systemd.daemon"] = "src/daemon.lua",
      ["systemd.daemon.core"] = {
         sources = { "src/daemon.c", "vendor/compat-5.2/c-api/compat-5.2.c" },
         libraries = { "systemd" },
         incdirs = { "vendor/compat-5.2/c-api/" }
      },
      ["systemd.id128"] = "src/id128.lua",
      ["systemd.id128.core"] = {
         sources = { "src/id128.c", "vendor/compat-5.2/c-api/compat-5.2.c" },
         libraries = { "systemd" },
         incdirs = { "vendor/compat-5.2/c-api/" }
      },
      ["systemd.journal"] = "src/journal.lua",
      ["systemd.journal.core"] = {
         sources = { "src/journal.c", "vendor/compat-5.2/c-api/compat-5.2.c" },
         libraries = { "systemd" },
         incdirs = { "vendor/compat-5.2/c-api/" }
      },
      ["systemd.login"] = "src/login.lua",
      ["systemd.login.core"] = {
         sources = { "src/login.c", "vendor/compat-5.2/c-api/compat-5.2.c" },
         libraries = { "systemd" },
         incdirs = { "vendor/compat-5.2/c-api/" }
      }
   }
}
