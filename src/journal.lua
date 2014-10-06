local c = require "systemd.journal.core"
local methods = c.JOURNAL_METHODS

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
		return res:match("=(.*)$")
	elseif ok == false then
		return nil
	else
		error(res);
	end
end

local function next_field(self)
	local ok, res = self:enumerate_data()
	if ok then
		return res:match("^([^=]*)=(.*)$")
	elseif ok == false then
		return nil
	else
		error(res)
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

return c
