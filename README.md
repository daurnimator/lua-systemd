[Systemd](http://freedesktop.org/wiki/Software/systemd/) for Lua.

Some trickier functions are bound in C.

## Done

C                       | Lua
------------------------|------------------------------------
`SD_LISTEN_FDS_START`   | `systemd.daemon.LISTEN_FDS_START`
`sd_notify()`           | `systemd.daemon.notify()`
`sd_pid_notify()`       | `systemd.daemon.pid_notify()`
`sd_booted()`           | `systemd.daemon.booted()`
`sd_journal_sendv()`    | `systemd.journal.sendv()`
`sd_journal_perror()`   | `systemd.journal.perror()`
`SD_JOURNAL_NOP`        | `systemd.journal.wakeup.NOP`
`SD_JOURNAL_APPEND`     | `systemd.journal.wakeup.APPEND`
`SD_JOURNAL_INVALIDATE` | `systemd.journal.wakeup.INVALIDATE`
