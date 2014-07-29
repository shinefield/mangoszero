Change log for mangos-zero
==========================

This change log references the relevant changes (bug and security fixes) done
in 1.0 minor versions.

## 1.0.0 (2014-08-XX)

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
* database: the commands `goname`/`namego` have been renamed to `appear`/`summon`,
* documentation has been moved to [Read The Docs](http://readthedocs.org/) and
  the latest version is available on [docs.getmangos.com](http://docs.getmangos.com),
* Eluna Lua Engine: thanks to the [Eluna project](https://github.com/ElunaLuaEngine)
  we now have working support for scripting based on [Lua 5.2.3](http://lua.org),
* platforms: support for compiling using clang on FreeBSD has been added,
* tools: all map tools have received documentation, their parameters have been
  cleaned up, and you will now see map version information,
* tools: the movement map extractor now sports an up to date list of junk and
  battleground maps,
* tools: all map tools now are able to skip junk maps, such as development or
  Azshara Crater.

Also numerous minor fixes and improvements have been added, such as:

* dependencies: [RecastNavigation](http://github.com/memononen/recastnavigation)
  has been updated to **SVN revision 256** (further updates pending),
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
* core: SMSG_AUTH_RESPONSE for full queue
* database: the honor commands had their access levels reset,
* database: the flying command has been removed (lacking support in vanilla WoW),
* tools: the list of contributed tools has been slimmed, as we killed of
  various unused tools, and the `git_id` helper has been removed,
* documentation: code documentation has been updated in various parts, to get
  rid of *Engrish* and other language variants.
* various enumeration for flags have been updated and/or properly documented.
* by default the world server will now mark itself as offline before initialization
  to prevent connections before it is ready,
