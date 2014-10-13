#!/usr/bin/env lua
--[[
This program acts like journalctl --list-boots
]]
local sj = require "systemd.journal"
local j = assert(sj.open())

local t = {}
local n = 0
for boot_id in j:each_unique("_BOOT_ID") do
	local boot_info = {
		id = boot_id;
	}
	n = n + 1
	t[n] = boot_info

	-- We need to find the first and last entries for each boot
	assert(j:add_match("_BOOT_ID="..boot_id))

	assert(j:seek_head())
	assert(j:next())
	boot_info.head = j:get_realtime_usec()

	assert(j:seek_tail())
	assert(j:previous())
	boot_info.tail = j:get_realtime_usec()

	j:flush_matches()
end

table.sort(t, function(a,b) return a.head < b.head end)

for i=1, n do
	local boot_info = t[i]
	io.write(string.format("%2d %s %s—%s\n",
		i-n, boot_info.id,
		os.date("%a %Y-%m-%d %H:%M:%S %Z", boot_info.head/1e6),
		os.date("%a %Y-%m-%d %H:%M:%S %Z", boot_info.tail/1e6)
	))
end
