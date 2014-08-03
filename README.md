# [Systemd](http://freedesktop.org/wiki/Software/systemd/) for Lua.

This library is for working with systemd from scripts and daemons written in Lua.

Where necessary, the low level `libsystemd` functions have been bound in C.
Higher level functions with more idiomatic lua semantics are written in Lua on top of these C primitives.

Compatible with Lua 5.1 and 5.2 (thanks [compat-5.2](https://github.com/hishamhm/lua-compat-5.2)).

# Installation

lua-systemd is on moonrocks: https://rocks.moonscript.org/modules/daurnimator/systemd

Install via luarocks: `luarocks install --server=http://rocks.moonscript.org systemd`

# Usage

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
`sd_journal_stream_fd()`| `systemd.journal.stream_fd()`
`SD_JOURNAL_NOP`        | `systemd.journal.WAKEUP.NOP`
`SD_JOURNAL_APPEND`     | `systemd.journal.WAKEUP.APPEND`
`SD_JOURNAL_INVALIDATE` | `systemd.journal.WAKEUP.INVALIDATE`

## Misc extras

### `systemd.journal.LOG`

Table containing the `syslog(3)` priority constants: `EMERG`, `ALERT`, `CRIT`, `ERR`, `WARNING`, `NOTICE`, `INFO`, `DEBUG`


### `systemd.journal.print(priority, fmt_string, ...)`

Same argument signature as C, but written in lua on top of `sendv()` and `string.format()`


### `systemd.journal.sendt(tbl)`

Log a message to the journal with the key/value pairs from `tbl`

