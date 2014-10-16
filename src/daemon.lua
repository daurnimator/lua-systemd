local c = require "systemd.daemon.core"

-- Wrap notify functions with versions that take tables
-- this lets you write `notifyt { READY=1, STATE="awesome!" }`
local function pack_state(t)
	local state = { }
	for k, v in pairs(t) do
		state[#state+1] = k.."="..v
	end
	return table.concat(state, "\n")
end
function c.notifyt(t)
	return c.notify(false, pack_state(t))
end
function c.pid_notifyt(pid, t)
	return c.pid_notify(pid, false, pack_state(t))
end

-- Get our own pid in pure lua.
local function get_pid()
	local fd, err, n = io.open "/proc/self/stat"
	if fd == nil then return nil, err, n end
	local pid = fd:read "*n"
	fd:close()
	return pid
end

-- sd_watchdog_enabled in pure lua.
-- returns watchdog interval
function c.watchdog_enabled(unset_environment)
	if unset_environment then error("unset not supported", 2) end

	local pid = os.getenv "WATCHDOG_PID"
	if not pid then return false end
	pid = tonumber(pid)
	if pid ~= get_pid() then return false end

	local usec = os.getenv "WATCHDOG_USEC"
	usec = tonumber(usec)
	if usec == nil then return nil, "invalid interval" end
	return usec/1e6
end

function c.kick_dog()
	return c.notify(false, "WATCHDOG=1")
end

return c
