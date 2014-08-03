# [Systemd](http://freedesktop.org/wiki/Software/systemd/) for Lua.

This library is for working with systemd from scripts and daemons written in Lua.

Where necessary, the low level `libsystemd` functions have been bound in C.
Higher level functions with more idiomatic lua semantics are written in Lua on top of these C primitives.

## Bound from C

All functions return `nil, error_message` in case of error.

C                       | Lua
------------------------|------------------------------------
`SD_LISTEN_FDS_START`   | `systemd.daemon.LISTEN_FDS_START`
`sd_notify()`           | `systemd.daemon.notify()`
`sd_pid_notify()`       | `systemd.daemon.pid_notify()`
`sd_booted()`           | `systemd.daemon.booted()`
`sd_journal_sendv()`    | `systemd.journal.sendv()`
`sd_journal_perror()`   | `systemd.journal.perror()`
`SD_JOURNAL_NOP`        | `systemd.journal.WAKEUP.NOP`
`SD_JOURNAL_APPEND`     | `systemd.journal.WAKEUP.APPEND`
`SD_JOURNAL_INVALIDATE` | `systemd.journal.WAKEUP.INVALIDATE`
