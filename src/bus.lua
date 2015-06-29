local c = require "systemd.bus.core"

function c.BUS_ERROR_METATABLE:__tostring()
	return string.format("sd_bus_error: %s: %s",
		self:get_name(),
		self:get_message()
	)
end

return c
