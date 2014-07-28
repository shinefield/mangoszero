Database Updates
================

This folder contains SQL files which will apply required updates to your MySQL
database, whenever the MaNGOS database structure has been changed or extended.

To see if you need an update, the file names have been given a fixed structure
that should enable you to see if you need an update or not.

=== File Name Description ===

File names are divided into two parts.

First part is the order_number, revision and counter that shows the commit revision that
will be compatible with the database after apply that particular update.
Counter sets the order to apply SQL updates for the same revision. Order_number set
global order in directory for backported SQL updates (its backported in random order
and need special mangos-zero side order for apply).

The second part of the name is the database and the table that needs an update or has been added.

See an example below:

         z0001_xxx_01_characters_character_db_version.sql
         |     |   |  |          |
         |     |   |  |          |
         |     |   |  |          The table `character_db_version`
         |     |   |  |          will need an update.
         |     |   |  |
         |     |   |  Name of affected DB (default recommended name)
         |     |   |  Can be: characters, mangos, realmd
         |     |   |
         |     |   Counter show number of SQL update in 0.12 backported or own (non backported, xxx) updates list for provided master revision
         |     |   and set proper order for SQL updates for same revision
         |     |
         |     MaNGOS 0.12 commit revision related to SQL update (if this backported SQL update.
         |     It can be absent (and replaced by xxx) if SQL update not really backported but only mangos-zero specific.
         |
         Order_number set special order for apply SQL updates from different backported revisions in mangos-zero


After applying an update the DB is compatible with the mangos revision of this SQL update.
SQL updates include special protection against multiple and wrong order of update application.

Attempts to apply SQL updates to an older DB without previous SQL updates in list for the database
or to DB with already applied this or later SQL update will generate an error and not be applied.

=== For Commiters ====

===== For backporting SQL updates =====

1. Rename SQL update from mangos-0.12 form (s0070_7932_01_characters_character_pet.sql) to mangos-zero form (z9999_s0070_01_characters_character_pet.sql)
   using any free order_number in sql/updates files list.

   For realmd SQL update you need use master revision instead mangos-0.12 (realmd revisions master based):
   (s0070_10008_01_realmd.sql) to mangos-zero form (z9999_10008_01_realmd.sql)

2. You can ignore SQL order guards in SQL update file.
   For make possible test code part build you can resolve revision_sql.h conflicts in any way.
   Work test better do to commit when revision_sql.h will regenerated.
   You also need resolve conflicts in DB SQL file (used revision in DB version not important)

===== For new SQL updates =====

1. Name SQL update in mangos-zero form (z9999_xxxxxx_01_characters_character_pet.sql)
   using any free order_number in sql/updates files list.

2. Add SQL update to commit and manually include SQL update changes in to related DB SQL file.
