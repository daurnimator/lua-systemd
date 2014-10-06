local c = require "systemd.id128.core"
local methods = c.ID128_METHODS

c.randomise = c.randomize

return c
