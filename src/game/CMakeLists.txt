# mangos-zero is a full featured server for World of Warcraft in its vanilla
# version, supporting clients for patch 1.12.x.
#
# Copyright (C) 2005-2014  MaNGOS project  <http://getmangos.com>
# Copyright (C) 2013-2014  CMaNGOS project <http://cmangos.net>
#
# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# ***** END GPL LICENSE BLOCK *****
#
# World of Warcraft, and all World of Warcraft or Warcraft art, images,
# and lore are copyrighted by Blizzard Entertainment, Inc.

include(MacroMangosSourceGroup)

#-----------------------------------------------------------------------------
# Build the mangos vmap library
add_subdirectory(vmap)

#-----------------------------------------------------------------------------
# Build the mangos battleground library
add_subdirectory(BattleGround)

#-----------------------------------------------------------------------------
# Build the mangos outdoor PvP library
add_subdirectory(OutdoorPvP)

#-----------------------------------------------------------------------------
# Build the mangos movement library
add_subdirectory(movement)

#-----------------------------------------------------------------------------
# Build the Eluna Lua scripting library
add_subdirectory(Eluna)

#-----------------------------------------------------------------------------
# Define the game library
set(sources
    AccountMgr.cpp
    AddonHandler.cpp
    AggressorAI.cpp
    AuctionHouseHandler.cpp
    AuctionHouseMgr.cpp
    Bag.cpp
    Camera.cpp
    Channel.cpp
    ChannelHandler.cpp
    ChannelMgr.cpp
    CharacterDatabaseCleaner.cpp
    CharacterHandler.cpp
    Chat.cpp
    ChatHandler.cpp
    CombatHandler.cpp
    ConfusedMovementGenerator.cpp
    Corpse.cpp
    CreatureAI.cpp
    CreatureAIRegistry.cpp
    CreatureAISelector.cpp
    Creature.cpp
    CreatureEventAI.cpp
    CreatureEventAIMgr.cpp
    CreatureLinkingMgr.cpp
    DBCStores.cpp
    debugcmds.cpp
    DuelHandler.cpp
    DynamicObject.cpp
    FleeingMovementGenerator.cpp
    FollowerReference.cpp
    GameEventMgr.cpp
    GameObject.cpp
    GMTicketHandler.cpp
    GMTicketMgr.cpp
    GossipDef.cpp
    GridMap.cpp
    GridNotifiers.cpp
    GridStates.cpp
    Group.cpp
    GroupHandler.cpp
    GroupReference.cpp
    GuardAI.cpp
    Guild.cpp
    GuildHandler.cpp
    GuildMgr.cpp
    HomeMovementGenerator.cpp
    HostileRefManager.cpp
    IdleMovementGenerator.cpp
    InstanceData.cpp
    Item.cpp
    ItemEnchantmentMgr.cpp
    ItemHandler.cpp
    LFGHandler.cpp
    LFGMgr.cpp
    Level0.cpp
    Level1.cpp
    Level2.cpp
    Level3.cpp
    LootHandler.cpp
    LootMgr.cpp
    Mail.cpp
    MailHandler.cpp
    Map.cpp
    MapManager.cpp
    MapPersistentStateMgr.cpp
    MassMailMgr.cpp
    MiscHandler.cpp
    MotionMaster.cpp
    MoveMap.cpp
    MovementGenerator.cpp
    MovementHandler.cpp
    NPCHandler.cpp
    NullCreatureAI.cpp
    ObjectAccessor.cpp
    Object.cpp
    ObjectGridLoader.cpp
    ObjectGuid.cpp
    ObjectMgr.cpp
    ObjectPosSelector.cpp
    Opcodes.cpp
    PathFinder.cpp
    PetAI.cpp
    Pet.cpp
    PetHandler.cpp
    PetitionsHandler.cpp
    Player.cpp
    PlayerDump.cpp
    PointMovementGenerator.cpp
    PoolManager.cpp
    QueryHandler.cpp
    QuestDef.cpp
    QuestHandler.cpp
    RandomMovementGenerator.cpp
    ReactorAI.cpp
    ReputationMgr.cpp
    ScriptMgr.cpp
    SkillHandler.cpp
    SocialMgr.cpp
    SpellAuras.cpp
    Spell.cpp
    SpellEffects.cpp
    SpellHandler.cpp
    SpellMgr.cpp
    SQLStorages.cpp
    StatSystem.cpp
    TargetedMovementGenerator.cpp
    TaxiHandler.cpp
    TemporarySummon.cpp
    ThreatManager.cpp
    TotemAI.cpp
    Totem.cpp
    TradeHandler.cpp
    Transports.cpp
    UnitAuraProcHandler.cpp
    Unit.cpp
    UpdateData.cpp
    WaypointManager.cpp
    WaypointMovementGenerator.cpp
    Weather.cpp
    World.cpp
    WorldSession.cpp
    WorldSocket.cpp
    WorldSocketMgr.cpp
)

set(headers
    AccountMgr.h
    AddonHandler.h
    AggressorAI.h
    AuctionHouseMgr.h
    Bag.h
    Camera.h
    Cell.h
    CellImpl.h
    Channel.h
    ChannelMgr.h
    CharacterDatabaseCleaner.h
    Chat.h
    ConfusedMovementGenerator.h
    Corpse.h
    CreatureAI.h
    CreatureAIImpl.h
    CreatureAIRegistry.h
    CreatureAISelector.h
    CreatureEventAI.h
    CreatureEventAIMgr.h
    Creature.h
    CreatureLinkingMgr.h
    DBCEnums.h
    DBCfmt.h
    DBCStores.h
    DBCStructure.h
    DynamicObject.h
    FleeingMovementGenerator.h
    FollowerReference.h
    FollowerRefManager.h
    Formulas.h
    GameEventMgr.h
    GameObject.h
    GMTicketMgr.h
    GossipDef.h
    GridDefines.h
    GridMap.h
    GridNotifiers.h
    GridNotifiersImpl.h
    GridStates.h
    Group.h
    GroupReference.h
    GroupRefManager.h
    GuardAI.h
    Guild.h
    GuildMgr.h
    HomeMovementGenerator.h
    HostileRefManager.h
    IdleMovementGenerator.h
    InstanceData.h
    ItemEnchantmentMgr.h
    Item.h
    ItemPrototype.h
    LFGHandler.h
    LFGMgr.h
    Language.h
    LootMgr.h
    Mail.h
    Map.h
    MapManager.h
    MapPersistentStateMgr.h
    MapReference.h
    MapRefManager.h
    MassMailMgr.h
    MotionMaster.h
    MoveMap.h
    MoveMapSharedDefines.h
    MovementGenerator.h
    MovementGeneratorImpl.h
    NPCHandler.h
    NullCreatureAI.h
    ObjectAccessor.h
    ObjectGridLoader.h
    ObjectGuid.h
    Object.h
    ObjectMgr.h
    ObjectPosSelector.h
    Opcodes.h
    PathFinder.h
    Path.h
    PetAI.h
    Pet.h
    PlayerDump.h
    Player.h
    PointMovementGenerator.h
    PoolManager.h
    QuestDef.h
    RandomMovementGenerator.h
    ReactorAI.h
    ReputationMgr.h
    ScriptMgr.h
    SharedDefines.h
    SocialMgr.h
    SpellAuraDefines.h
    SpellAuras.h
    Spell.h
    SpellMgr.h
    SQLStorages.h
    TargetedMovementGenerator.h
    TemporarySummon.h
    ThreatManager.h
    TotemAI.h
    Totem.h
    Transports.h
    UnitEvents.h
    Unit.h
    UpdateData.h
    UpdateFields.h
    UpdateMask.h
    WaypointManager.h
    WaypointMovementGenerator.h
    Weather.h
    World.h
    WorldSession.h
    WorldSocket.h
    WorldSocketMgr.h
)

set(game_LIB_SRCS ${sources} ${headers})

mangos_source_group(${game_LIB_SRCS})

include_directories(
    ${ACE_INCLUDE_DIRS}
    ${MYSQL_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/dep/include/g3dlite/
    ${LUA_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour/Include/
    ${CMAKE_SOURCE_DIR}/src/framework/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vmap/
    ${CMAKE_CURRENT_SOURCE_DIR}/BattleGround/
    ${CMAKE_CURRENT_SOURCE_DIR}/Eluna/
    ${CMAKE_CURRENT_SOURCE_DIR}/OutdoorPvP/
    ${CMAKE_CURRENT_SOURCE_DIR}/movement/
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src/framework/
)

#-----------------------------------------------------------------------------
# Build the game library
add_library(mangos-game STATIC ${game_LIB_SRCS})
target_link_libraries(mangos-game mangos-battleground mangos-eluna mangos-outdoor-pvp mangos-movement mangos-vmap mangos-framework Detour g3dlite ${ACE_LIBRARIES} ${MYSQL_LIBRARIES} ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES})

# Special case for mangos-scripts library builds
if(UNIX)
  # Both systems don't have libdl and don't need them
  if (NOT (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "NetBSD"))
    target_link_libraries(mangos-game dl)
  endif()
endif()

add_dependencies(mangos-game revision.h)

#-----------------------------------------------------------------------------
# Append compile flags - if set by build definition
set(BUILD_PROPERTIES "-DDT_POLYREF64")

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(BUILD_PROPERTIES "${BUILD_PROPERTIES} -DMANGOS_DEBUG")
endif()

if(WITH_DEBUG_OUTPUT)
    set(BUILD_PROPERTIES "${BUILD_PROPERTIES} -DVMAP_DEBUG")
endif()

if(WITH_MZ_SCRIPTS)
    set(BUILD_PROPERTIES "${BUILD_PROPERTIES} -DWITH_MOD_SCRIPTDEV2" )
endif()

set_target_properties(mangos-game PROPERTIES COMPILE_FLAGS ${BUILD_PROPERTIES})
