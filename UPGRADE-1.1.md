UPGRADE FROM 1.0 to 1.1
=======================

This document summarizes changes between release 1.0 and 1.1 version which require
your attention, and may require further action to adapt your installation of the
***mangos-zero** server.

## General

* Support for ScriptDev2 has been removed in favour of our own scripting engine
  [Eluna](http://eluna.demudevs.com/).
* Support for PostgreSQL has been dropped. IF you previously used this database
  backend, you will have to migrate to using MySQL or MariaDB.
* Lua 5.2 is now an external dependency. Please install Lua 5.2 and development
  headers. Windows users should grab [Lua binaries](http://luabinaries.sourceforge.net/)
  and Linux users will find a Lua 5.2 package from their distribution vendor, e.g.
  Debian/Ubuntu provide `liblua5.2-0`/`liblua5.2-dev` packages.

### SOAP

Support for SOAP has been removed. Thus the following configuration settings
have been removed from `mangosd.conf`:

* `SOAP.Enable`
* `SOAP.IP`
* `SOAP.Port`

### Remote Access

Support for Remote Access has been removed. Thus the following configuration
settings have been removed from `mangosd.conf`:

* `RaLogFile`
* `Ra.Enable`
* `Ra.IP`
* `Ra.Port`
* `Ra.MinLevel`
* `Ra.Secure`
* `Ra.Stricted`

## World Server

The configuration version has changed from `2013012201` to `2014110301`.

### Maps with active objects

Game data for inactive maps may now be kept in memory even when no players
are online. This is intended to e.g. keep creatures with waypoints on following
their defined waypoints.

A new setting `LoadAllGridsOnMaps` has been added to `mangosd.conf` which allows
to pass a string with a comma separated list of map identifiers for maps that
should be kept online, e.g.

    LoadAllGridsOnMaps="0,1"

To support this feature, `creature_template` entries in the world database
can now be tagged by setting `CREATURE_FLAG_EXTRA_ACTIVE` (`0x00001000`) in
the `ExtraFlags` for a template.

### Default log levels

The `LogLevel` setting for console logging and the `LogFileLevel` settings
for file logging have received new default settings.

Console logging is set to `1` for basic and error output, while file logging
has been set to `3` for detailed debug output.

This has been changed to provide user with maximum information on their first
runs. For production installation, we recommend to set both `LogLevel` and
`LogFileLevel` to `1`.

### Log filters

Changes to player values will no longer be printed to the console by default.
This the `LogFilter_PlayerStats` has been set to `1`.

### Eluna script directory

The `Eluna.ScriptPath` has been updated. Previously by default it was set to
a directory named `lua_scripts`. This has been changed to `scripts`.

### Battleground scores storage

The character database has been extended to facilitate room for storing score
statistics for players and battlegrounds. The following tables have been
added:

* `pvpstats_battlegrounds`
* `pvpstats_players`

The update script `z2484_s2204_12756_01_characters_pvpstats.sql` provides the
required SQL statements to upgrade your character database.

A new configuration setting `Battleground.ScoreStatistics` has been added
to `mangosd.conf` and is disabled by default. Setting the value to `1` will
enable logging of battleground and player statistics.
