local journal = require "systemd.journal"

function log(severity, msg, level)
	local info = debug.getinfo((level or 1)+1, "nlS")
	if info.currentline == -1 then
		info.currentline = nil
	end
	return assert(journal.sendt{
		CODE_FILE = info.short_src;
		CODE_FUNC = info.name;
		CODE_LINE = info.currentline;
		PRIORITY = journal.LOG[severity:upper()] or 5;
		MESSAGE = msg;
	})
end
