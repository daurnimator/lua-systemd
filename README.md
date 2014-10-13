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
  - [x] [Journal writing](http://www.freedesktop.org/software/systemd/man/sd_journal_sendv.html)
  - [x] Journal reading
  - [ ] [Journal change notification](http://www.freedesktop.org/software/systemd/man/sd_journal_get_fd.html)
  - [ ] [Login monitoring](http://www.freedesktop.org/software/systemd/man/sd_login_monitor.html) - Monitor login sessions, seats, users and virtual machines/containers
  - [ ] Unit control - Requires use of [dbus API](http://www.freedesktop.org/wiki/Software/systemd/dbus/)


# Installation

lua-systemd is on moonrocks: https://rocks.moonscript.org/modules/daurnimator/systemd

Install via luarocks: `luarocks install --server=http://rocks.moonscript.org/manifests/daurnimator systemd`


# Usage

## Bound from C

All functions return `nil, error_message [, errno]` in case of error.

 C                                                                                                                                       | Lua                                      | Comments
-----------------------------------------------------------------------------------------------------------------------------------------|------------------------------------------|----------------------------------------
[`SD_LISTEN_FDS_START`](http://www.freedesktop.org/software/systemd/man/SD_LISTEN_FDS_START.html)                                        | `systemd.daemon.LISTEN_FDS_START`        |
[`sd_notify()`](http://www.freedesktop.org/software/systemd/man/sd_notify.html)                                                          | `systemd.daemon.notify()`                |
[`sd_pid_notify()`](http://www.freedesktop.org/software/systemd/man/sd_pid_notify.html)                                                  | `systemd.daemon.pid_notify()`            |
[`sd_booted()`](http://www.freedesktop.org/software/systemd/man/sd_booted.html)                                                          | `systemd.daemon.booted()`                |
[`sd_journal_sendv()`](http://www.freedesktop.org/software/systemd/man/sd_journal_sendv.html)                                            | `systemd.journal.sendv()`                |
[`sd_journal_perror()`](http://www.freedesktop.org/software/systemd/man/sd_journal_perror.html)                                          | `systemd.journal.perror()`               |
[`sd_journal_stream_fd()`](http://www.freedesktop.org/software/systemd/man/sd_journal_stream_fd.html)                                    | `systemd.journal.stream_fd()`            | On success, returns a Lua `file` object instead of raw file descriptor
[`SD_JOURNAL_LOCAL_ONLY`](http://www.freedesktop.org/software/systemd/man/SD_JOURNAL_LOCAL_ONLY.html)                                    | `systemd.journal.OPEN.LOCAL_ONLY`        |
[`SD_JOURNAL_RUNTIME_ONLY`](http://www.freedesktop.org/software/systemd/man/SD_JOURNAL_RUNTIME_ONLY.html)                                | `systemd.journal.OPEN.RUNTIME_ONLY`      |
[`SD_JOURNAL_SYSTEM`](http://www.freedesktop.org/software/systemd/man/SD_JOURNAL_SYSTEM.html)                                            | `systemd.journal.OPEN.SYSTEM`            |
[`SD_JOURNAL_CURRENT_USER`](http://www.freedesktop.org/software/systemd/man/SD_JOURNAL_CURRENT_USER.html)                                | `systemd.journal.OPEN.CURRENT_USER`      |
[`sd_journal_open()`](http://www.freedesktop.org/software/systemd/man/sd_journal_open.html)                                              | `systemd.journal.open()`                 |
[`sd_journal_open_directory()`](http://www.freedesktop.org/software/systemd/man/sd_journal_open_directory.html)                          | `systemd.journal.open_directory()`       |
[`sd_journal_open_files()`](http://www.freedesktop.org/software/systemd/man/sd_journal_open_files.html)                                  | `systemd.journal.open_files()`           |
[`sd_journal_open_container()`](http://www.freedesktop.org/software/systemd/man/sd_journal_open_container.html)                          | `systemd.journal.open_container()`       |
[`sd_journal_get_cutoff_realtime_usec()`](http://www.freedesktop.org/software/systemd/man/sd_journal_get_cutoff_realtime_usec.html)      | `my_journal:get_cutoff_realtime_usec()`  |
[`sd_journal_get_cutoff_monotonic_usec()`](http://www.freedesktop.org/software/systemd/man/sd_journal_get_cutoff_monotonic_usec.html)    | `my_journal:get_cutoff_monotonic_usec()` |
[`sd_journal_next()`](http://www.freedesktop.org/software/systemd/man/sd_journal_next.html)                                              | `my_journal:next()`                      |
[`sd_journal_next_skip()`](http://www.freedesktop.org/software/systemd/man/sd_journal_next_skip.html)                                    | `my_journal:next_skip()`                 |
[`sd_journal_previous()`](http://www.freedesktop.org/software/systemd/man/sd_journal_previous.html)                                      | `my_journal:previous()`                  |
[`sd_journal_previous_skip()`](http://www.freedesktop.org/software/systemd/man/sd_journal_previous_skip.html)                            | `my_journal:previous_skip()`             |
[`sd_journal_seek_head()`](http://www.freedesktop.org/software/systemd/man/sd_journal_seek_head.html)                                    | `my_journal:seek_head()`                 |
[`sd_journal_seek_tail()`](http://www.freedesktop.org/software/systemd/man/sd_journal_seek_tail.html)                                    | `my_journal:seek_tail()`                 |
[`sd_journal_seek_monotonic_usec()`](http://www.freedesktop.org/software/systemd/man/sd_journal_seek_monotonic_usec.html)                | `my_journal:seek_monotonic_usec()`       |
[`sd_journal_seek_realtime_usec()`](http://www.freedesktop.org/software/systemd/man/sd_journal_seek_realtime_usec.html)                  | `my_journal:seek_realtime_usec()`        |
[`sd_journal_seek_cursor()`](http://www.freedesktop.org/software/systemd/man/sd_journal_seek_cursor.html)                                | `my_journal:seek_cursor()`               |
[`sd_journal_get_cursor()`](http://www.freedesktop.org/software/systemd/man/sd_journal_get_cursor.html)                                  | `my_journal:get_cursor()`                |
[`sd_journal_test_cursor()`](http://www.freedesktop.org/software/systemd/man/sd_journal_test_cursor.html)                                | `my_journal:test_cursor()`               |
[`sd_journal_get_realtime_usec()`](http://www.freedesktop.org/software/systemd/man/sd_journal_get_realtime_usec.html)                    | `my_journal:get_realtime_usec()`         |
[`sd_journal_get_monotonic_usec()`](http://www.freedesktop.org/software/systemd/man/sd_journal_get_monotonic_usec.html)                  | `my_journal:get_monotonic_usec()`        |
[`sd_journal_get_data()`](http://www.freedesktop.org/software/systemd/man/sd_journal_get_data.html)                                      | `my_journal:get_data()`                  |
[`sd_journal_enumerate_data()`](http://www.freedesktop.org/software/systemd/man/sd_journal_enumerate_data.html)                          | `my_journal:enumerate_data()`            |
[`sd_journal_restart_data()`](http://www.freedesktop.org/software/systemd/man/sd_journal_restart_data.html)                              | `my_journal:restart_data()`              |
[`sd_journal_query_unique()`](http://www.freedesktop.org/software/systemd/man/sd_journal_query_unique.html)                              | `my_journal:query_unique()`              |
[`sd_journal_enumerate_unique()`](http://www.freedesktop.org/software/systemd/man/sd_journal_enumerate_unique.html)                      | `my_journal:enumerate_unique()`          |
[`sd_journal_restart_unique()`](http://www.freedesktop.org/software/systemd/man/sd_journal_restart_unique.html)                          | `my_journal:restart_unique()`            |
[`sd_journal_set_data_threshold()`](http://www.freedesktop.org/software/systemd/man/sd_journal_set_data_threshold.html)                  | `my_journal:set_data_threshold()`        |
[`sd_journal_get_data_threshold()`](http://www.freedesktop.org/software/systemd/man/sd_journal_get_data_threshold.html)                  | `my_journal:get_data_threshold()`        |
[`sd_journal_add_match()`](http://www.freedesktop.org/software/systemd/man/sd_journal_add_match.html)                                    | `my_journal:add_match()`                 |
[`sd_journal_add_disjunction()`](http://www.freedesktop.org/software/systemd/man/sd_journal_add_disjunction.html)                        | `my_journal:add_disjunction()`           |
[`sd_journal_add_conjunction()`](http://www.freedesktop.org/software/systemd/man/sd_journal_add_conjunction.html)                        | `my_journal:add_conjunction()`           |
[`sd_journal_flush_matches()`](http://www.freedesktop.org/software/systemd/man/sd_journal_flush_matches.html)                            | `my_journal:flush_matches()`             |
[`SD_JOURNAL_NOP`](http://www.freedesktop.org/software/systemd/man/SD_JOURNAL_NOP.html)                                                  | `systemd.journal.WAKEUP.NOP`             |
[`SD_JOURNAL_APPEND`](http://www.freedesktop.org/software/systemd/man/SD_JOURNAL_APPEND.html)                                            | `systemd.journal.WAKEUP.APPEND`          |
[`SD_JOURNAL_INVALIDATE`](http://www.freedesktop.org/software/systemd/man/SD_JOURNAL_INVALIDATE.html)                                    | `systemd.journal.WAKEUP.INVALIDATE`      |
[`sd_journal_get_fd()`](http://www.freedesktop.org/software/systemd/man/sd_journal_get_fd.html)                                          | `my_journal:get_fd()`                    |
[`sd_journal_get_events()`](http://www.freedesktop.org/software/systemd/man/sd_journal_get_events.html)                                  | `my_journal:get_events()`                |
[`sd_journal_get_timeout()`](http://www.freedesktop.org/software/systemd/man/sd_journal_get_timeout.html)                                | `my_journal:get_timeout()`               | Operates in seconds instead of microseconds
[`sd_journal_process()`](http://www.freedesktop.org/software/systemd/man/sd_journal_process.html)                                        | `my_journal:process()`                   |
[`sd_journal_wait()`](http://www.freedesktop.org/software/systemd/man/sd_journal_wait.html)                                              | `my_journal:wait()`                      | `timeout` is in seconds instead of microseconds
[`sd_journal_reliable_fd()`](http://www.freedesktop.org/software/systemd/man/sd_journal_reliable_fd.html)                                | `my_journal:reliable_fd()`               |
[`sd_id128_randomize()`](http://www.freedesktop.org/software/systemd/man/sd_id128_randomize.html)                                        | `systemd.id128.randomize()`              |
[`sd_id128_from_string()`](http://www.freedesktop.org/software/systemd/man/sd_id128_from_string.html)                                    | `systemd.id128.from_string()`            |
[`sd_id128_get_machine()`](http://www.freedesktop.org/software/systemd/man/sd_id128_get_machine.html)                                    | `systemd.id128.get_machine()`            |
[`sd_id128_get_boot()`](http://www.freedesktop.org/software/systemd/man/sd_id128_get_boot.html)                                          | `systemd.id128.get_boot()`               |
[`sd_id128_to_string()`](http://www.freedesktop.org/software/systemd/man/sd_id128_to_string.html)                                        | `my_id128:to_string()`                   | Also available as `__tostring` metamethod: `tostring(my_id128_t)`
[`sd_id128_equal()`](http://www.freedesktop.org/software/systemd/man/sd_id128_equal.html)                                                | `id128_a == id128_b`                     | Bound as `__eq` metamethod
[`sd_journal_get_catalog_for_message_id()`](http://www.freedesktop.org/software/systemd/man/sd_journal_get_catalog_for_message_id.html)  | `my_id128:get_catalog()`                 |
[`sd_get_seats()`](http://www.freedesktop.org/software/systemd/man/sd_get_seats.html)                                                    | `systemd.login.get_seats()`              |
[`sd_get_sessions()`](http://www.freedesktop.org/software/systemd/man/sd_get_sessions.html)                                              | `systemd.login.get_sessions()`           |
[`sd_get_uids()`](http://www.freedesktop.org/software/systemd/man/sd_get_uids.html)                                                      | `systemd.login.get_uids()`               |
[`sd_get_machine_names()`](http://www.freedesktop.org/software/systemd/man/sd_get_machine_names.html)                                    | `systemd.login.get_machine_names()`      |
[`sd_pid_get_session()`](http://www.freedesktop.org/software/systemd/man/sd_pid_get_session.html)                                        | `systemd.login.pid_get_session()`        |
[`sd_pid_get_unit()`](http://www.freedesktop.org/software/systemd/man/sd_pid_get_unit.html)                                              | `systemd.login.pid_get_unit()`           |
[`sd_pid_get_user_unit()`](http://www.freedesktop.org/software/systemd/man/sd_pid_get_user_unit.html)                                    | `systemd.login.pid_get_user_unit()`      |
[`sd_pid_get_owner_uid()`](http://www.freedesktop.org/software/systemd/man/sd_pid_get_owner_uid.html)                                    | `systemd.login.pid_get_owner_uid()`      |
[`sd_pid_get_machine_name()`](http://www.freedesktop.org/software/systemd/man/sd_pid_get_machine_name.html)                              | `systemd.login.pid_get_machine_name()`   |


## Misc extras

### `systemd.daemon.(pid_)notifyt(tbl)`

Like `notify`, but takes a lua table instead of a newline delimited list.
Numbers will be coerced to strings.

```lua
notifyt { READY = 1, STATUS = "Server now accepting connections", WATCHDOG = 1 }
```


### `interval = systemd.daemon.watchdog_enabled()`

Returns the watchdog interval (in seconds) if there is one set otherwise returns `false`.

You should call `kick_dog` or `notify("WATCHDOG=1")` every half of this interval.

Similar functionality to [`sd_watchdog_enabled`](http://www.freedesktop.org/software/systemd/man/sd_watchdog_enabled.html)


### `systemd.daemon.kick_dog()`

Tells systemd to update the watchdog timestamp.
This should be called on an interval.


### `systemd.journal.LOG`

Table containing the `syslog(3)` priority constants: `EMERG`, `ALERT`, `CRIT`, `ERR`, `WARNING`, `NOTICE`, `INFO`, `DEBUG`

Useful as the second argument to `systemd.journal.streamfd()`


### `systemd.journal.print(priority, fmt_string, ...)`

Same argument signature as C, but written in lua on top of `sendv()` and `string.format()`


### `systemd.journal.sendt(tbl)`

Log a message to the journal with the key/value pairs from `tbl`


### `value = my_journal:get(field)`

Returns the given field from the current journal entry (which may be `nil`)

Throws a lua error on failure.


### `my_journal:each_data()`

A valid lua iterator that enumerates through field, value pairs.

```lua
for field, value in my_journal:each_data() do
	print(field, value)
end
```

Throws a lua error on failure.


### `my_journal:each_unique()`

A valid lua iterator that enumerates through unique field values.

This snippet prints each of the different `_SYSTEMD_UNIT`s in the given journal:

```lua
for value in my_journal:each_unique("_SYSTEMD_UNIT") do
	print(value)
end
```

Throws a lua error on failure.


### `t = my_journal:to_table()`

Converts the current journal entry to a lua table.

Includes [Address Fields](http://www.freedesktop.org/software/systemd/man/systemd.journal-fields.html#Address%20Fields):
  - `__CURSOR`
  - `__REALTIME_TIMESTAMP`
  - `__MONOTOPIC_TIMESTAMP`


### `text = my_journal:get_catalog()`

Looks up the current journal entry's `MESSAGE_ID` in the message catalog.
Substitutes the templated items (between `@` symbols) with values from this journal entry.

Returns the filled out catalogue entry as a string; `false` if `MESSAGE_ID` is not set, or does not exist in the catalogue; or `nil, err_msg, errno` in case of failure.

Same functionality as [`sd_journal_get_catalog()`](http://www.freedesktop.org/software/systemd/man/sd_journal_get_catalog.html).
