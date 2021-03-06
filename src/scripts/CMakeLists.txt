# mangos-zero is a full featured server for World of Warcraft in its vanilla
# version, supporting clients for patch 1.12.x.
#
# Copyright (C) 2005-2014  MaNGOS project  <http://getmangos.com>
# Parts Copyright (C) 2006-2014  ScriptDev2 <http://scriptdev2.com/>
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
# Options
option(WITH_SCRIPTDEV2_EXAMPLES             "Build the example script"    OFF)
mark_as_advanced(WITH_SCRIPTDEV2_EXAMPLES)

#-----------------------------------------------------------------------------
# Define the scriptdev2 library
set(sources
    sd2_revision_nr.h
    sd2_revision_sql.h
    ScriptDevMgr.cpp
    ScriptDevMgr.h
    base/escort_ai.cpp
    base/escort_ai.h
    base/follower_ai.cpp
    base/follower_ai.h
    base/guard_ai.cpp
    base/guard_ai.h
    base/pet_ai.cpp
    base/pet_ai.h

    include/sc_creature.cpp
    include/sc_creature.h
    include/sc_grid_searchers.cpp
    include/sc_grid_searchers.h
    include/sc_instance.cpp
    include/sc_instance.h
    include/sc_gossip.h

    system/MangosdRev.cpp
    system/ScriptLoader.cpp
    system/ScriptLoader.h
    system/system.cpp
    system/system.h
)

# set(sources_battlegrounds
#     scripts/battlegrounds/battleground.cpp
# )

set(sources_examples
    scripts/examples/examplecreature.cpp
    scripts/examples/exampleescort.cpp
    scripts/examples/examplegossip_codebox.cpp
    scripts/examples/examplemisc.cpp
)

# set(sources_world
#     scripts/world/areatrigger_scripts.cpp
#     scripts/world/bosses_emerald_dragons.cpp
#     scripts/world/go_scripts.cpp
#     scripts/world/guards.cpp
#     scripts/world/item_scripts.cpp
#     scripts/world/mob_generic_creature.cpp
#     scripts/world/npc_professions.cpp
#     scripts/world/npcs_special.cpp
#     scripts/world/spell_scripts.cpp
#     scripts/world/world_map_scripts.cpp
#     scripts/world/world_map_scripts.h
# )

# set(sources_eastern_kingdoms
#     scripts/eastern_kingdoms/alterac_mountains.cpp
#     scripts/eastern_kingdoms/arathi_highlands.cpp
#     scripts/eastern_kingdoms/blasted_lands.cpp
#     scripts/eastern_kingdoms/boss_kazzak.cpp
#     scripts/eastern_kingdoms/burning_steppes.cpp
#     scripts/eastern_kingdoms/dun_morogh.cpp
#     scripts/eastern_kingdoms/eastern_plaguelands.cpp
#     scripts/eastern_kingdoms/elwynn_forest.cpp
#     scripts/eastern_kingdoms/hinterlands.cpp
#     scripts/eastern_kingdoms/ironforge.cpp
#     scripts/eastern_kingdoms/loch_modan.cpp
#     scripts/eastern_kingdoms/redridge_mountains.cpp
#     scripts/eastern_kingdoms/searing_gorge.cpp
#     scripts/eastern_kingdoms/silverpine_forest.cpp
#     scripts/eastern_kingdoms/stormwind_city.cpp
#     scripts/eastern_kingdoms/stranglethorn_vale.cpp
#     scripts/eastern_kingdoms/swamp_of_sorrows.cpp
#     scripts/eastern_kingdoms/tirisfal_glades.cpp
#     scripts/eastern_kingdoms/undercity.cpp
#     scripts/eastern_kingdoms/western_plaguelands.cpp
#     scripts/eastern_kingdoms/westfall.cpp
#     scripts/eastern_kingdoms/wetlands.cpp
# )

# set(sources_kalimdor
#     scripts/kalimdor/ashenvale.cpp
#     scripts/kalimdor/azshara.cpp
#     scripts/kalimdor/boss_azuregos.cpp
#     scripts/kalimdor/darkshore.cpp
#     scripts/kalimdor/desolace.cpp
#     scripts/kalimdor/durotar.cpp
#     scripts/kalimdor/dustwallow_marsh.cpp
#     scripts/kalimdor/felwood.cpp
#     scripts/kalimdor/feralas.cpp
#     scripts/kalimdor/moonglade.cpp
#     scripts/kalimdor/mulgore.cpp
#     scripts/kalimdor/orgrimmar.cpp
#     scripts/kalimdor/silithus.cpp
#     scripts/kalimdor/stonetalon_mountains.cpp
#     scripts/kalimdor/tanaris.cpp
#     scripts/kalimdor/teldrassil.cpp
#     scripts/kalimdor/the_barrens.cpp
#     scripts/kalimdor/thousand_needles.cpp
#     scripts/kalimdor/thunder_bluff.cpp
#     scripts/kalimdor/ungoro_crater.cpp
#     scripts/kalimdor/winterspring.cpp
# )

# set(sources_instance_ek_brd
#     scripts/eastern_kingdoms/blackrock_depths/blackrock_depths.cpp
#     scripts/eastern_kingdoms/blackrock_depths/blackrock_depths.h
#     scripts/eastern_kingdoms/blackrock_depths/boss_ambassador_flamelash.cpp
#     scripts/eastern_kingdoms/blackrock_depths/boss_emperor_dagran_thaurissan.cpp
#     scripts/eastern_kingdoms/blackrock_depths/boss_general_angerforge.cpp
#     scripts/eastern_kingdoms/blackrock_depths/boss_high_interrogator_gerstahn.cpp
#     scripts/eastern_kingdoms/blackrock_depths/instance_blackrock_depths.cpp
# )

# set(sources_instance_ek_brs
#     scripts/eastern_kingdoms/blackrock_spire/blackrock_spire.h
#     scripts/eastern_kingdoms/blackrock_spire/boss_gyth.cpp
#     scripts/eastern_kingdoms/blackrock_spire/boss_overlord_wyrmthalak.cpp
#     scripts/eastern_kingdoms/blackrock_spire/boss_pyroguard_emberseer.cpp
#     scripts/eastern_kingdoms/blackrock_spire/instance_blackrock_spire.cpp
# )

# set(sources_instance_ek_tdm
#     scripts/eastern_kingdoms/deadmines/boss_mr_smite.cpp
#     scripts/eastern_kingdoms/deadmines/deadmines.cpp
#     scripts/eastern_kingdoms/deadmines/deadmines.h
#     scripts/eastern_kingdoms/deadmines/instance_deadmines.cpp
# )

# set(sources_instance_ek_gno
#     scripts/eastern_kingdoms/gnomeregan/boss_thermaplugg.cpp
#     scripts/eastern_kingdoms/gnomeregan/gnomeregan.cpp
#     scripts/eastern_kingdoms/gnomeregan/gnomeregan.h
#     scripts/eastern_kingdoms/gnomeregan/instance_gnomeregan.cpp
# )

# set(sources_instance_ek_sm
#     scripts/eastern_kingdoms/scarlet_monastery/boss_arcanist_doan.cpp
#     scripts/eastern_kingdoms/scarlet_monastery/boss_herod.cpp
#     scripts/eastern_kingdoms/scarlet_monastery/boss_mograine_and_whitemane.cpp
#     scripts/eastern_kingdoms/scarlet_monastery/instance_scarlet_monastery.cpp
#     scripts/eastern_kingdoms/scarlet_monastery/scarlet_monastery.h
# )

# set(sources_instance_ek_scholo
#     scripts/eastern_kingdoms/scholomance/boss_darkmaster_gandling.cpp
#     scripts/eastern_kingdoms/scholomance/boss_jandice_barov.cpp
#     scripts/eastern_kingdoms/scholomance/instance_scholomance.cpp
#     scripts/eastern_kingdoms/scholomance/scholomance.h
# )

# set(sources_instance_ek_sfk
#     scripts/eastern_kingdoms/shadowfang_keep/instance_shadowfang_keep.cpp
#     scripts/eastern_kingdoms/shadowfang_keep/shadowfang_keep.cpp
#     scripts/eastern_kingdoms/shadowfang_keep/shadowfang_keep.h
# )

# set(sources_instance_ek_strat
#     scripts/eastern_kingdoms/stratholme/boss_baroness_anastari.cpp
#     scripts/eastern_kingdoms/stratholme/boss_cannon_master_willey.cpp
#     scripts/eastern_kingdoms/stratholme/boss_dathrohan_balnazzar.cpp
#     scripts/eastern_kingdoms/stratholme/boss_maleki_the_pallid.cpp
#     scripts/eastern_kingdoms/stratholme/instance_stratholme.cpp
#     scripts/eastern_kingdoms/stratholme/stratholme.cpp
#     scripts/eastern_kingdoms/stratholme/stratholme.h
# )

# set(sources_instance_ek_st
#     scripts/eastern_kingdoms/sunken_temple/instance_sunken_temple.cpp
#     scripts/eastern_kingdoms/sunken_temple/sunken_temple.cpp
#     scripts/eastern_kingdoms/sunken_temple/sunken_temple.h
# )

# set(sources_instance_ek_uld
#     scripts/eastern_kingdoms/uldaman/boss_archaedas.cpp
#     scripts/eastern_kingdoms/uldaman/instance_uldaman.cpp
#     scripts/eastern_kingdoms/uldaman/uldaman.cpp
#     scripts/eastern_kingdoms/uldaman/uldaman.h
# )

# set(sources_raid_ek_mc
#     scripts/eastern_kingdoms/molten_core/boss_baron_geddon.cpp
#     scripts/eastern_kingdoms/molten_core/boss_garr.cpp
#     scripts/eastern_kingdoms/molten_core/boss_gehennas.cpp
#     scripts/eastern_kingdoms/molten_core/boss_golemagg.cpp
#     scripts/eastern_kingdoms/molten_core/boss_lucifron.cpp
#     scripts/eastern_kingdoms/molten_core/boss_magmadar.cpp
#     scripts/eastern_kingdoms/molten_core/boss_majordomo_executus.cpp
#     scripts/eastern_kingdoms/molten_core/boss_ragnaros.cpp
#     scripts/eastern_kingdoms/molten_core/boss_shazzrah.cpp
#     scripts/eastern_kingdoms/molten_core/boss_sulfuron_harbinger.cpp
#     scripts/eastern_kingdoms/molten_core/instance_molten_core.cpp
#     scripts/eastern_kingdoms/molten_core/molten_core.cpp
#     scripts/eastern_kingdoms/molten_core/molten_core.h
# )

# set(sources_raid_ek_bwl
#     scripts/eastern_kingdoms/blackwing_lair/blackwing_lair.h
#     scripts/eastern_kingdoms/blackwing_lair/boss_broodlord_lashlayer.cpp
#     scripts/eastern_kingdoms/blackwing_lair/boss_chromaggus.cpp
#     scripts/eastern_kingdoms/blackwing_lair/boss_ebonroc.cpp
#     scripts/eastern_kingdoms/blackwing_lair/boss_firemaw.cpp
#     scripts/eastern_kingdoms/blackwing_lair/boss_flamegor.cpp
#     scripts/eastern_kingdoms/blackwing_lair/boss_nefarian.cpp
#     scripts/eastern_kingdoms/blackwing_lair/boss_razorgore.cpp
#     scripts/eastern_kingdoms/blackwing_lair/boss_vaelastrasz.cpp
#     scripts/eastern_kingdoms/blackwing_lair/boss_victor_nefarius.cpp
#     scripts/eastern_kingdoms/blackwing_lair/instance_blackwing_lair.cpp
# )

# set(sources_raid_ek_naxx
#     scripts/eastern_kingdoms/naxxramas/boss_anubrekhan.cpp
#     scripts/eastern_kingdoms/naxxramas/boss_faerlina.cpp
#     scripts/eastern_kingdoms/naxxramas/boss_four_horsemen.cpp
#     scripts/eastern_kingdoms/naxxramas/boss_gluth.cpp
#     scripts/eastern_kingdoms/naxxramas/boss_gothik.cpp
#     scripts/eastern_kingdoms/naxxramas/boss_grobbulus.cpp
#     scripts/eastern_kingdoms/naxxramas/boss_heigan.cpp
#     scripts/eastern_kingdoms/naxxramas/boss_kelthuzad.cpp
#     scripts/eastern_kingdoms/naxxramas/boss_loatheb.cpp
#     scripts/eastern_kingdoms/naxxramas/boss_maexxna.cpp
#     scripts/eastern_kingdoms/naxxramas/boss_noth.cpp
#     scripts/eastern_kingdoms/naxxramas/boss_patchwerk.cpp
#     scripts/eastern_kingdoms/naxxramas/boss_razuvious.cpp
#     scripts/eastern_kingdoms/naxxramas/boss_sapphiron.cpp
#     scripts/eastern_kingdoms/naxxramas/boss_thaddius.cpp
#     scripts/eastern_kingdoms/naxxramas/instance_naxxramas.cpp
#     scripts/eastern_kingdoms/naxxramas/naxxramas.h
# )

# set(sources_raid_ek_zg
#     scripts/eastern_kingdoms/zulgurub/boss_arlokk.cpp
#     scripts/eastern_kingdoms/zulgurub/boss_hakkar.cpp
#     scripts/eastern_kingdoms/zulgurub/boss_hazzarah.cpp
#     scripts/eastern_kingdoms/zulgurub/boss_jeklik.cpp
#     scripts/eastern_kingdoms/zulgurub/boss_jindo.cpp
#     scripts/eastern_kingdoms/zulgurub/boss_mandokir.cpp
#     scripts/eastern_kingdoms/zulgurub/boss_marli.cpp
#     scripts/eastern_kingdoms/zulgurub/boss_renataki.cpp
#     scripts/eastern_kingdoms/zulgurub/boss_thekal.cpp
#     scripts/eastern_kingdoms/zulgurub/boss_venoxis.cpp
#     scripts/eastern_kingdoms/zulgurub/instance_zulgurub.cpp
#     scripts/eastern_kingdoms/zulgurub/zulgurub.h
# )

# set(sources_instance_k_bdf
#     scripts/kalimdor/blackfathom_deeps/blackfathom_deeps.h
#     scripts/kalimdor/blackfathom_deeps/instance_blackfathom_deeps.cpp
# )

# set(sources_instance_k_dm
#     scripts/kalimdor/dire_maul/dire_maul.cpp
#     scripts/kalimdor/dire_maul/dire_maul.h
#     scripts/kalimdor/dire_maul/instance_dire_maul.cpp
# )

# set(sources_instance_k_mara
#     scripts/kalimdor/maraudon/boss_noxxion.cpp
#     scripts/kalimdor/maraudon/maraudon.cpp
#     scripts/kalimdor/maraudon/maraudon.h
# )

# set(sources_instance_k_rfd
#     scripts/kalimdor/razorfen_downs/razorfen_downs.cpp
#     scripts/kalimdor/razorfen_downs/razorfen_downs.h
# )

# set(sources_instance_k_rfk
#     scripts/kalimdor/razorfen_kraul/instance_razorfen_kraul.cpp
#     scripts/kalimdor/razorfen_kraul/razorfen_kraul.cpp
#     scripts/kalimdor/razorfen_kraul/razorfen_kraul.h
# )

# set(sources_instance_k_wc
#     scripts/kalimdor/wailing_caverns/instance_wailing_caverns.cpp
#     scripts/kalimdor/wailing_caverns/wailing_caverns.cpp
#     scripts/kalimdor/wailing_caverns/wailing_caverns.h
# )

# set(sources_instance_k_zf
#     scripts/kalimdor/zulfarrak/boss_zumrah.cpp
#     scripts/kalimdor/zulfarrak/instance_zulfarrak.cpp
#     scripts/kalimdor/zulfarrak/zulfarrak.cpp
#     scripts/kalimdor/zulfarrak/zulfarrak.h
# )

# set(sources_raid_k_ony
#     scripts/kalimdor/onyxias_lair/boss_onyxia.cpp
#     scripts/kalimdor/onyxias_lair/instance_onyxias_lair.cpp
#     scripts/kalimdor/onyxias_lair/onyxias_lair.h
# )

# set(sources_raid_k_raq
#     scripts/kalimdor/ruins_of_ahnqiraj/boss_ayamiss.cpp
#     scripts/kalimdor/ruins_of_ahnqiraj/boss_buru.cpp
#     scripts/kalimdor/ruins_of_ahnqiraj/boss_kurinnaxx.cpp
#     scripts/kalimdor/ruins_of_ahnqiraj/boss_moam.cpp
#     scripts/kalimdor/ruins_of_ahnqiraj/boss_ossirian.cpp
#     scripts/kalimdor/ruins_of_ahnqiraj/boss_rajaxx.cpp
#     scripts/kalimdor/ruins_of_ahnqiraj/instance_ruins_of_ahnqiraj.cpp
#     scripts/kalimdor/ruins_of_ahnqiraj/ruins_of_ahnqiraj.cpp
#     scripts/kalimdor/ruins_of_ahnqiraj/ruins_of_ahnqiraj.h
# )

# set(sources_raid_k_taq
#     scripts/kalimdor/temple_of_ahnqiraj/boss_bug_trio.cpp
#     scripts/kalimdor/temple_of_ahnqiraj/boss_cthun.cpp
#     scripts/kalimdor/temple_of_ahnqiraj/boss_fankriss.cpp
#     scripts/kalimdor/temple_of_ahnqiraj/boss_huhuran.cpp
#     scripts/kalimdor/temple_of_ahnqiraj/boss_ouro.cpp
#     scripts/kalimdor/temple_of_ahnqiraj/boss_sartura.cpp
#     scripts/kalimdor/temple_of_ahnqiraj/boss_skeram.cpp
#     scripts/kalimdor/temple_of_ahnqiraj/boss_twinemperors.cpp
#     scripts/kalimdor/temple_of_ahnqiraj/boss_viscidus.cpp
#     scripts/kalimdor/temple_of_ahnqiraj/instance_temple_of_ahnqiraj.cpp
#     scripts/kalimdor/temple_of_ahnqiraj/mob_anubisath_sentinel.cpp
#     scripts/kalimdor/temple_of_ahnqiraj/temple_of_ahnqiraj.h
# )

set(mangosscript_LIB_SRCS
    ${sources}
#   ${sources_battlegrounds}
#   ${sources_world}
#   ${sources_eastern_kingdoms}
#   ${sources_kalimdor}
#   ${sources_instance_ek_brd}
#   ${sources_instance_ek_brs}
#   ${sources_instance_ek_tdm}
#   ${sources_instance_ek_gno}
#   ${sources_instance_ek_sm}
#   ${sources_instance_ek_scholo}
#   ${sources_instance_ek_sfk}
#   ${sources_instance_ek_strat}
#   ${sources_instance_ek_st}
#   ${sources_instance_ek_uld}
#   ${sources_raid_ek_mc}
#   ${sources_raid_ek_bwl}
#   ${sources_raid_ek_naxx}
#   ${sources_raid_ek_zg}
#   ${sources_instance_k_bdf}
#   ${sources_instance_k_dm}
#   ${sources_instance_k_mara}
#   ${sources_instance_k_rfd}
#   ${sources_instance_k_rfk}
#   ${sources_instance_k_wc}
#   ${sources_instance_k_zf}
#   ${sources_raid_k_ony}
#   ${sources_raid_k_raq}
#   ${sources_raid_k_taq}
)

if(WITH_SCRIPTDEV2_EXAMPLES)
    list(APPEND mangosscript_LIB_SRCS
        ${sources_examples}
     )
endif()

mangos_source_group(${mangosscript_LIB_SRCS})

# mangos_source_group_topic(${sources_battlegrounds} "Battlegrounds")
# mangos_source_group_topic(${sources_examples} "Examples")
# mangos_source_group_topic(${sources_world} "Generic")
# mangos_source_group_topic(${sources_eastern_kingdoms} "Eastern Kingdoms")
# mangos_source_group_topic(${sources_kalimdor} "Kalimdor")
# mangos_source_group_topic(${sources_instance_ek_brd} "Instances\\Blackrock Depths")
# mangos_source_group_topic(${sources_instance_ek_brs} "Instances\\Blackrock Spire")
# mangos_source_group_topic(${sources_instance_ek_tdm} "Instances\\The Deadmines")
# mangos_source_group_topic(${sources_instance_ek_gno} "Instances\\Gnomeregan")
# mangos_source_group_topic(${sources_instance_ek_sm} "Instances\\Scarlet Monastery")
# mangos_source_group_topic(${sources_instance_ek_scholo} "Instances\\Scholomance")
# mangos_source_group_topic(${sources_instance_ek_sfk} "Instances\\Shadowfang Keep")
# mangos_source_group_topic(${sources_instance_ek_strat} "Instances\\Stratholme")
# mangos_source_group_topic(${sources_instance_ek_st} "Instances\\Sunken Temple")
# mangos_source_group_topic(${sources_instance_ek_uld} "Instances\\Uldaman")
# mangos_source_group_topic(${sources_raid_ek_mc} "Raids\\Molten Core")
# mangos_source_group_topic(${sources_raid_ek_bwl} "Raids\\Blackwing Lair")
# mangos_source_group_topic(${sources_raid_ek_naxx} "Raids\\Naxxramas")
# mangos_source_group_topic(${sources_raid_ek_zg} "Raids\\Zul Gurub")
# mangos_source_group_topic(${sources_instance_k_bdf} "Instances\\Blackfathom Deeps")
# mangos_source_group_topic(${sources_instance_k_dm} "Instances\\Dire Maul")
# mangos_source_group_topic(${sources_instance_k_mara} "Instances\\Maraudon")
# mangos_source_group_topic(${sources_instance_k_rfd} "Instances\\Razorfen Downs")
# mangos_source_group_topic(${sources_instance_k_rfk} "Instances\\Razorfen Kraul")
# mangos_source_group_topic(${sources_instance_k_wc} "Instances\\Wailing Caverns")
# mangos_source_group_topic(${sources_instance_k_zf} "Instances\\Zul Farrak")
# mangos_source_group_topic(${sources_raid_k_ony} "Raids\\Onyxias Lair")
# mangos_source_group_topic(${sources_raid_k_raq} "Raids\\Ruins of AhnQiraj")
# mangos_source_group_topic(${sources_raid_k_taq} "Raids\\Temple of AhnQiraj")

include_directories(
    ${ACE_INCLUDE_DIRS}
    ${MYSQL_INCLUDE_DIR}
    # ${OPENSSL_INCLUDE_DIR}
    # ${ZLIB_INCLUDE_DIR}
    # ${CMAKE_SOURCE_DIR}/dep/include/g3dlite/
    # ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour/Include/
    ${CMAKE_SOURCE_DIR}/src/framework/
    ${CMAKE_SOURCE_DIR}/src/game/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/base/
    ${CMAKE_CURRENT_SOURCE_DIR}/system/
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src/framework/
)

#-----------------------------------------------------------------------------
# Dump generated headers and add custom target for version control information

set(SD2_CONF_VERSION_MAJOR   "2012")
set(SD2_CONF_VERSION_MINOR   "11")
set(SD2_CONF_VERSION_RELEASE "23")
set(SD2_CONF_VERSION_COUNT   "01")
set(SCRIPTDEV2_VERSION       "${SD2_CONF_VERSION_MAJOR}${SD2_CONF_VERSION_MINOR}${SD2_CONF_VERSION_RELEASE}${SD2_CONF_VERSION_COUNT}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-sd2.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-sd2.h)

#-----------------------------------------------------------------------------
# Handle SQL files and updates
add_subdirectory(sql)

#-----------------------------------------------------------------------------
# Build the scriptdev2 library
add_library(mangos-scripts SHARED ${mangosscript_LIB_SRCS})

set(BUILD_PROPERTIES " ")
if(WITH_SCRIPTDEV2_EXAMPLES)
    set(BUILD_PROPERTIES "${BUILD_PROPERTIES} -DENABLE_SD2_EXAMPLES")
endif()
set_target_properties(mangos-scripts PROPERTIES COMPILE_FLAGS ${BUILD_PROPERTIES})

#-----------------------------------------------------------------------------
# Link the scriptdev2 library
#
# NOTE: this a hack, required for Windows to make it possible to actually use
#       the library. This will be dropped once we have Lua working.
if(WIN32 AND MSVC)
    target_link_libraries(mangos-scripts mangosd ${ACE_LIBRARIES}) # ${MYSQL_LIBRARIES} ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES}
endif()

#-----------------------------------------------------------------------------
# Build and install the scriptdev2 files
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/scriptdev2.conf.dist.cmake ${CMAKE_CURRENT_BINARY_DIR}/scriptdev2.conf.dist)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/scriptdev2.conf.dist DESTINATION ${CONF_DIR})

install(TARGETS mangos-scripts
    DESTINATION ${LIBS_DIR}
    LIBRARY DESTINATION ${LIBS_DIR}
)
