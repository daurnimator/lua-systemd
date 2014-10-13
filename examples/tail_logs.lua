#!/usr/bin/env lua
--[[
This script will tail your journal.
Output will be in json format.

Uses the cqueues library (see http://25thandclement.com/~william/projects/cqueues.html)
]]

local cqueues = require "cqueues"
local bit = require "bit32"
local json = require "dkjson"
local sj = require "systemd.journal"

local j = assert(sj.open())
assert(j:seek_tail())
assert(j:previous())

-- Wrap the journal object with an object that implements the cqueues.poll interface
local wrap_journal do
	local methods = {}
	function methods:pollfd()
		return self.j:get_fd()
	end
	function methods:events()
		local mask = self.j:get_events()
		local events = ""
		if bit.band(mask, 1) ~= 0 then
			events = events .. "r"
		end
		if bit.band(mask, 4) ~= 0 then
			events = events .. "w"
		end
		return events
	end
	function methods:timeout()
		local m = self.j:get_timeout()
		if type(m) == "number" then
			local now = cqueues.monotime()
			return m - now;
		end
	end
	local mt = {
		__index = function(t,k)
			local f = methods[k]
			if f ~= nil then return f end
			-- Delegate to original journal methods
			local f = t.j[k]
			if type(f) == "function" then
				-- Need to make sure the correct 'self' is passed through
				return function(o, ...)
					if o == t then
						o = t.j
					end
					return f(o, ...)
				end
			end
		end;
	}
	function wrap_journal(j)
		return setmetatable({j = j}, mt)
	end
end
local w = wrap_journal(j)

local q = cqueues.new()
q:wrap(function()
	while true do
		local a = cqueues.poll(w)
		if a and a:process() ~= sj.WAKEUP.NOP then
			while a:next() do
				local t = assert(a:to_table())
				print(json.encode(t))
			end
		end
	end
end)
assert(q:loop())
