local c = require "systemd.journal.core"

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

return c
