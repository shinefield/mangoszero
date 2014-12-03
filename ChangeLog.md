Change log for mangos-zero
==========================

This change log references the relevant changes (bug and security fixes) done
in 1.0 minor versions.

## 1.2.0 (2014-12-XX)

Major changes for this build which require your attention when upgrading include
things such as these:

*

Minor fixes and improvements have been added, such as:

*

## 1.1.0 (2014-12-03)

Major changes for this build which require your attention when upgrading include
things such as these:

* support for ScriptDev2 has been dropped, and the matching configuration options
  are gone. Instead we provide our own script library, where we recreate scripts
  for game content from scratch (use WITH_MZ_SCRIPTS parameter to enable/disable).
* fixed resting system. Resting will now work as intended.
* SOAP bindings removed! In order to focus our efforts on the server and increasing
  its' quality, will continue to strip down the server to relevant components.
* remote access removed! To reduce possible security risk, we stripped the remote
  access console from the world server.
* **new** configuration setting `LoadAllGridsOnMaps`: with this setting a list of
  maps can be defined on which grids and creatures will be kept loaded. Continents
  will be loaded on server start, instances when a player enters. **Notice** that
  this will increase CPU load and memory usage depending on the amount of maps you
  chose to keep in memory.
* load active entities based on active maps. Ideas and loop to load all grids on
  a map by Neo2003.
* implement `CREATURE_FLAG_EXTRA_ACTIVE`: setting this flag in `creature_template`
  `ExtraFlags` will result in the creature being loaded and set to active when the
  map is created. Use with care.
* added a new admin command to force raid instance resets
* implement new TEMPFACTION_TOGGLE flags for `UNIT_FLAG_PACIFIED` and
  `UNIT_FLAG_NOT_SELECTABLE`
* implement TARGET_UNIT_NEARBY_ALLY and unify handling of TARGET_UNIT_NEARBY_
* allow target 60 to use script target when required
* remove invisibility aura (18) based on attributes. Passive and negative auras will
  not be removed on combat
* implement spell effect 19395
* implement Battleground scores storage system
* implement EVENT_T_ENERGY for EventAI
* SCRIPT_COMMAND_CAST_SPELL (15) allows passing additional spell-IDs with
  dataint..dataint4 columns. If these are used, a random spell out of datalong,
  dataint, ..dataintX is used as cast spell.
* Lua 5.2 is now an external dependency. Please install Lua 5.2 with development
  headers. Windows users should grab [Lua binaries](http://luabinaries.sourceforge.net/)
  and Linux users will find a Lua 5.2 package from their distribution vendor, e.g.
  Debian/Ubuntu provide `liblua5.2-0`/`liblua5.2-dev` packages.

Minor fixes and improvements have been added, such as:

* the CMake build system is now compatible with CMake 2.8 and CMake 3.
* fixed the range of traps based on YouTube videos.
* fixed the range in which spell focus objects (such as fires) will trigger.
  This prevents fire damage taken even though you're not standing in the fire.
* database files and updates will now be installed, too.
* code refactoring in many functions makes the code more readable.
* rooted players will now not only appear rooted to other players, but actually
  stay rooted for real.
* fixed skinning loot bug.
* allow non-instanced empty map creation.
* replaced `Map::LoadGrid` with `Map::ForceLoadGrid`
* fixed raid instance reset crash
* use strict database filter for creatures with movement points without a
  matching movement type
* fix possible mount abuse after leaving battlegrounds
* unify startup messages, and changed loading routines for sane
  grouping
* use a global team index for battleground teams
* fixed a bug that caused AreaAura reapply because the code doesn't search the
  correct rank of it.
* fixed AreaAura bug when low level grouped with high level Player. Thank to
  @tarwyn for his precious help and proposed patch.
* allow logout during duels
* various code style fixes
* various performance fixes
* CreatureLinking: improve startup efficiency. No need to use multiple select to
  check and obtain information about db-data of an expectedly unique creature
* CreatureLinking: allow recursive CanSpawn checks. This will fix behaviour of
  mobs whose spawning is dependent on mobs whose spawning is dependent on other
  mobs.

## 1.0.0 (2014-10-27)

Minor fixes and improvements have been added, such as:

* usernames are now normalized when updating the account password, no longer
  resulting in the creation of a broken SHA1 hash due to lowercase characters
  in usernames.
* an unused setting for max character level has been removed.
* paths will no longer be recalculated on movement speed changing effects on
  creatures. This removes stuttering when creatures are e.g. hit by Frost Bolt.
* removed support for [Shiv](http://wowpedia.org/Shiv) which was introduced
  in The Burning Crusade first.
* grouped players will now see stealthed members with the proper animation.
* fixed build for Visual Studio 2012 (/bigobj required).
* the build system is now fully compatible with CMake 3.0, and less spammy
  when building Lua.

## 1.0.0-RC2 (2014-10-16)

Major changes for this build which require your attention when upgrading include
things such as these:

* removal of PostgreSQL support. Sadly, due to lack of testing and development
  resources required to maintain proper support, we had to remove support for
  PostgreSQL.
* removal of the auction house bot. Sadly, due to lack of testing and development
  resources required to maintain proper support, we had to remove support for
  the auction house bot.

Also numerous minor fixes and improvements have been added, such as:

* the overall build time for mangos-zero has been greatly reduced, and support
  for building on multiple CPU cores is considered final now. Enjoy an awesome,
  fast build with CMake!
* Windows builds have been fixed. Oops!

## 1.0.0-RC1 (2014-10-10)

Major changes for this build which require your attention when upgrading include
awesome things such as these:

* build system: we now supply a [CMake](http://cmake.org/) only build system, which
  can be used to export project files for the compiler / IDE of your choice. We
  support Linux, Windows, Mac OS X and FreeBSD,
* configuration: the default configurations for the realm and world servers, and the
  Auction House bot have been redacted, and now sport readable English, and - due to
  the new build system - will already include settings for paths and components,
* core: the libraries known as `shared` and `framework` library now have engaged in
  a more intimate relationship, and have been merged into the `mangos-framework`
  library,
* core: support for meeting stones has been added, making players able to queue
  as intended,
* core: distance for trade windows has been doubled,
* database: the table `scripted_event_id` has been renamed to `scripted_event` in the
  world server database,
* core: dispel effects will now check if there is anything to dispel first and fail
  to cast with no mana cost if no effects are to be dispelled,
* core: creature linking has a new flag to despawn linked creatures on master
  despawn,
* core: vmap extraction will now use bounding geometry when extracting model
  geometry which fixes various LOS issues,
* core: mmap generation now uses an up to date version for recastnavigation, and
  has been tweaked to achieve better results,
* core: the recast demo application now is able to preview generated data
  visually,
* core: random movement will now switch between short and long reset timers,
* core: polymorphed or feared creatures will no longer attack or chase,
* core: trap game objects will despawn with their parent game object,
* EventAI: added ACTION_T_SUMMON_UNIQUE for unique creature summons,
* scripts: added SCRIPT_COMMAND_TURN_TO for turning units to face a target,
* quest: when quests reward spell casts, they will now display the correct spell,
* documentation has been moved to [Read The Docs](http://readthedocs.org/) and
  the latest version is available on [docs.getmangos.com](http://docs.getmangos.com),
* Eluna Lua Engine: thanks to the [Eluna project](https://github.com/ElunaLuaEngine)
  we now have working support for scripting based on [Lua 5.2.3](http://lua.org)
  which is enabled by default and ready for you to enjoy,
* platforms: support for compiling using clang on FreeBSD has been added,
* tools: all map tools have received documentation, their parameters have been
  cleaned up, and you will now see map version information,
* tools: the movement map extractor now sports an up to date list of junk and
  battleground maps,
* tools: all map tools now are able to skip junk maps, such as development or
  Azshara Crater,
* tools: map tools will now always be built, and are no longer an optional part
  of the build process.

Also numerous minor fixes and improvements have been added, such as:

* dependencies: [RecastNavigation](http://github.com/memononen/recastnavigation)
  has been upgraded to use the latest release,
* core: various memory leaks and compiler warnings have been fixed,
* core: home location for players will no longer reset to a wrong location,
* core: character creation will only allow race/class combinations as defined
  in the game client's DBC files,
* core: duelling will not crash the server if a player disconnects before
  duelling ends,
* core: item prototypes, spell families, quest definitions, and movement flags
  have been updated,
* core: the `npc unfollow` command and the `debug send opcode` commands have
  been fixed,
* core: sound effects being played for zones without weather definitions have been
  fixed,
* core: quest givers which reward spells will now ignore cast power requirements,
* core: overwriting higher level heals over time with lower ranks is no longer
  possible,
* core: Arcane Power fixed,
* core: Combustion fixed,
* core: rage ticking fixed,
* core: item cooldowns can no longer be reset by removing/equipping the item as
  the items' cooldown will now be considered,
* core: CMSG_PAGE_TEXT_QUERY parsing,
* core: fixed reconnecting to the world,
* core: fixed SMSG_LOGOUT_RESPONSE,
* core: SMSG_AUTH_RESPONSE for full queue,
* core: spell family max corrected to 13 (aka potions),
* core: chat channel spam is gone,
* database: the commands `goname`/`namego` have been renamed to `appear`/`summon`,
* database: the honor commands had their access levels reset,
* database: the flying command has been removed (lacking support in vanilla WoW),
* tools: the list of contributed tools has been slimmed, as we killed of
  various unused tools, and the `git_id` helper has been removed,
* documentation: code documentation has been updated in various parts, to get
  rid of *Engrish* and other language variants.
* various enumeration for flags have been updated and/or properly documented.
* by default the world server will now mark itself as offline before initialization
  to prevent connections before it is ready,
