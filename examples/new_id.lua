--[[
This program should act just like running `journalctl --new-id128`
]]

local id128 = require "systemd.id128"

local myuuid = tostring(assert(id128.randomise()))

io.write(table.concat({
	"As string:",
	myuuid,
	"",
	"As UUID:",
	myuuid:gsub("(........)(....)(....)(............)", "%1-%2-%3-%4"),
	"",
	"As macro:",
	"#define MESSAGE_XYZ SD_ID128_MAKE(" .. myuuid:gsub("(..)", "%1,"):sub(1,-2) .. ")",
	"",
	"As Python constant:",
	">>> import uuid",
	">>> MESSAGE_XYZ = uuid.UUID('" .. myuuid .. "')",
	""
},"\n"))

