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

return c
