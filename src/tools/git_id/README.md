Git revision helper
===================

`git_id` is a helper tool for those looking to contribute to *mangos-zero*.
By default, mangos-zero happily lives with git revision hashes, but for all
things database related, we use revision numbers we hand-craft into a change
whenever a backwards incompatible change is made, or any of the databases is
modified.

`git_id` will fetch the latest of these revision numbers, and update your
local copy with a new number.

Currently this will update the files `src/shared/revision_nr.h` for core
changes, and `src/shared/revision_sql.h` for database changes.
