local id128 = require "systemd.id128"

local c = require "systemd.journal.core"
local methods = c.JOURNAL_METHODS

local function strip_field_name(str)
	return str:match("=(.*)$")
end

local function split_field(str)
	return str:match("^([^=]*)=(.*)$")
end

c.LOG = {
	EMERG = 0;
	ALERT = 1;
	CRIT = 2;
	ERR = 3;
	WARNING = 4;
	NOTICE = 5;
	INFO = 6;
	DEBUG = 7;
}

function c.print(priority, ...)
	return c.sendv {
		"PRIORITY=" .. priority;
		"MESSAGE=" .. string.format(...);
	}
end

function c.sendt(m)
	local t = { }
	for k, v in pairs(m) do
		t[#t+1] = k .. "=" .. v
	end
	return c.sendv(t)
end

function methods:get(field)
	local ok, res, code = self:get_data(field)
	if ok then
		return strip_field_name(res)
	elseif ok == false then
		return nil
	else
		error(res, 2);
	end
end

local function next_field(self)
	local ok, res = self:enumerate_data()
	if ok then
		return split_field(res)
	elseif ok == false then
		return nil
	else
		error(res, 2)
	end
end
function methods:each_data()
	self:restart_data()
	return next_field, self
end

-- Converts the current journal entry to a lua table
-- Includes __ prefixed "Address Fields"
function methods:to_table()
	local t = {
		__CURSOR = self:get_cursor();
		__REALTIME_TIMESTAMP = self:get_realtime_usec();
		__MONOTONIC_TIMESTAMP = self:get_monotonic_usec();
	}
	for field, value in self:each_data() do
		t[field] = value
	end
	return t
end

function methods:get_catalog()
	local ok, err, errno = self:get_data "MESSAGE_ID"
	if ok == nil then
		return nil, err, errno
	elseif ok == false then
		return false
	else
		local message_id = err:match("=(.*)$")
		if message_id == nil then return false end
		local message_id = id128.from_string(message_id)
		local text = message_id:get_catalog()
		if not text then return false end
		local t = self:to_table()
		text = text:gsub("@([^@]-)@", t)
		return text
	end
end

return c
