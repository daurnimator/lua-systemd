# [Systemd](http://freedesktop.org/wiki/Software/systemd/) for Lua.

This library is for working with systemd from scripts and daemons written in Lua.

Where necessary, the low level `libsystemd` functions have been bound in C.
Higher level functions with more idiomatic lua semantics are written in Lua on top of these C primitives.

Compatible with Lua 5.1 and 5.2 (thanks [compat-5.2](https://github.com/hishamhm/lua-compat-5.2)).


# Status

Waiting for API to stabilise before making an initial release.


## Todo

  - [x] [Notify](http://www.freedesktop.org/software/systemd/man/sd_notify.html) - Notify service manager about start-up completion and other daemon status changes
  - [x] ID128 - APIs for processing 128-bit IDs
  - [x] Journal writing
  - [ ] Journal reading
  - [ ] [Login monitoring](http://www.freedesktop.org/software/systemd/man/sd_login_monitor.html) - Monitor login sessions, seats, users and virtual machines/containers
  - [ ] Unit control - Requires use of [dbus API](http://www.freedesktop.org/wiki/Software/systemd/dbus/)


# Installation

lua-systemd is on moonrocks: https://rocks.moonscript.org/modules/daurnimator/systemd

Install via luarocks: `luarocks install --server=http://rocks.moonscript.org/manifests/daurnimator systemd`


# Usage

## Bound from C

All functions return `nil, error_message [, errno]` in case of error.

C                                         | Lua
------------------------------------------|------------------------------------
`SD_LISTEN_FDS_START`                     | `systemd.daemon.LISTEN_FDS_START`
`sd_notify()`                             | `systemd.daemon.notify()`
`sd_pid_notify()`                         | `systemd.daemon.pid_notify()`
`sd_booted()`                             | `systemd.daemon.booted()`
`sd_journal_sendv()`                      | `systemd.journal.sendv()`
`sd_journal_perror()`                     | `systemd.journal.perror()`
`sd_journal_stream_fd()`                  | `systemd.journal.stream_fd()`
`SD_JOURNAL_NOP`                          | `systemd.journal.WAKEUP.NOP`
`SD_JOURNAL_APPEND`                       | `systemd.journal.WAKEUP.APPEND`
`SD_JOURNAL_INVALIDATE`                   | `systemd.journal.WAKEUP.INVALIDATE`
`sd_journal_open()`                       | `systemd.journal.open()`
`sd_journal_open_directory()`             | `systemd.journal.open_directory()`
`sd_journal_open_files()`                 | `systemd.journal.open_files()`
`sd_journal_open_container()`             | `systemd.journal.open_container()
`SD_JOURNAL_LOCAL_ONLY`                   | `systemd.journal.OPEN.LOCAL_ONLY`
`SD_JOURNAL_RUNTIME_ONLY`                 | `systemd.journal.OPEN.RUNTIME_ONLY`
`SD_JOURNAL_SYSTEM`                       | `systemd.journal.OPEN.SYSTEM`
`SD_JOURNAL_CURRENT_USER`                 | `systemd.journal.OPEN.CURRENT_USER`
`sd_journal_next()`                       | `my_journal:next()`
`sd_journal_next_skip()`                  | `my_journal:next_skip()`
`sd_journal_previous()`                   | `my_journal:previous()`
`sd_journal_previous_skip()`              | `my_journal:previous_skip()`
`sd_journal_seek_head()`                  | `my_journal:seek_head()`
`sd_journal_seek_tail()`                  | `my_journal:seek_tail()`
`sd_journal_seek_monotonic_usec()`        | `my_journal:seek_monotonic_usec()`
`sd_journal_seek_realtime_usec()`         | `my_journal:seek_realtime_usec()`
`sd_journal_seek_cursor()`                | `my_journal:seek_cursor()`
`sd_journal_get_realtime_usec()`          | `my_journal:get_realtime_usec()`
`sd_journal_get_monotonic_usec()`         | `my_journal:get_monotonic_usec()`
`sd_journal_get_data()`                   | `my_journal:get_data()`
`sd_journal_enumerate_data()`             | `my_journal:enumerate_data()`
`sd_journal_restart_data()`               | `my_journal:restart_data()`
`sd_journal_set_data_threshold()`         | `my_journal:set_data_threshold()`
`sd_journal_get_data_threshold()`         | `my_journal:get_data_threshold()`
`sd_journal_add_match()`                  | `my_journal:add_match()`
`sd_journal_add_disjunction()`            | `my_journal:add_disjunction()`
`sd_journal_add_conjunction()`            | `my_journal:add_conjunction()`
`sd_journal_flush_matches()`              | `my_journal:flush_matches()`
`sd_id128_randomize()`                    | `systemd.id128.randomize()`
`sd_id128_from_string()`                  | `systemd.id128.from_string()`
`sd_id128_get_machine()`                  | `systemd.id128.get_machine()`
`sd_id128_get_boot()`                     | `systemd.id128.get_boot()`
`sd_id128_to_string()`                    | `tostring(my_id128_t)`
`sd_id128_equal()`                        | `id128_a == id128_b`
`sd_get_seats()`                          | `systemd.login.get_seats()`
`sd_get_sessions()`                       | `systemd.login.get_sessions()`
`sd_get_uids()`                           | `systemd.login.get_uids()`
`sd_get_machine_names()`                  | `systemd.login.get_machine_names()`
`sd_pid_get_session()`                    | `systemd.login.pid_get_session()`
`sd_pid_get_unit()`                       | `systemd.login.pid_get_unit()`
`sd_pid_get_user_unit()`                  | `systemd.login.pid_get_user_unit()`
`sd_pid_get_owner_uid()`                  | `systemd.login.pid_get_owner_uid()`
`sd_pid_get_machine_name()`               | `systemd.login.pid_get_machine_name()`

## Misc extras

### `systemd.daemon.(pid_)notifyt(tbl)`

Like `notify`, but takes a lua table instead of a newline delimited list.

```lua
notifyt { READY = 1, STATUS = "Server now accepting connections", WATCHDOG = 1 }
```


### `interval = systemd.daemon.watchdog_enabled()`

Returns the watchdog interval (in seconds) if there is one set otherwise returns `false`.

You should call `kick_dog` or `notify("WATCHDOG=1")` every half of this interval.


### `systemd.daemon.kick_dog()`

Tells systemd to update the watchdog timestamp.
This should be called on an interval.


### `systemd.journal.LOG`

Table containing the `syslog(3)` priority constants: `EMERG`, `ALERT`, `CRIT`, `ERR`, `WARNING`, `NOTICE`, `INFO`, `DEBUG`


### `systemd.journal.print(priority, fmt_string, ...)`

Same argument signature as C, but written in lua on top of `sendv()` and `string.format()`


### `systemd.journal.sendt(tbl)`

Log a message to the journal with the key/value pairs from `tbl`


### `value = my_journal:get(field)`

Returns the given field from the current journal entry (which may be nil)

Throws a lua error on failure.


### `my_journal:each_data()`

A valid lua iterator that enumerates through field, value pairs.

```lua
for field, value in my_journal:each_data() do
	print(field, value)
end
```

Throws a lua error on failure.
