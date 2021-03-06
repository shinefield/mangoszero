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
# Define the mangosd application
set(sources
    Main.cpp
    Master.cpp
    WorldRunnable.cpp
    CliRunnable.cpp
)
set(headers
    Master.h
    WorldRunnable.h
    CliRunnable.h
)

set(mangosd_LIB_SRCS ${sources} ${headers})

mangos_source_group(${mangosd_LIB_SRCS})

include_directories(
    ${ACE_INCLUDE_DIRS}
    ${MYSQL_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/src/framework/
    ${CMAKE_SOURCE_DIR}/src/game/
    ${CMAKE_SOURCE_DIR}/src/game/Eluna/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src/framework/
)

#-----------------------------------------------------------------------------
# Configure the mangosd application
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mangosd.conf.dist.cmake ${CMAKE_CURRENT_BINARY_DIR}/mangosd.conf.dist)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mods.conf.dist.cmake ${CMAKE_CURRENT_BINARY_DIR}/mods.conf.dist)

#-----------------------------------------------------------------------------
# Build the mangosd application
if(WIN32 AND MSVC)
    add_executable(mangosd "mangosd.rc" ${mangosd_LIB_SRCS})
else()
    add_executable(mangosd ${mangosd_LIB_SRCS})
endif()
target_link_libraries(mangosd ${ACE_LIBRARIES} mangos-game mangos-battleground mangos-outdoor-pvp mangos-movement mangos-vmap mangos-framework Detour g3dlite ${MYSQL_LIBRARIES} ${OPENSSL_LIBRARIES})
install(TARGETS mangosd DESTINATION ${BIN_DIR})

add_dependencies(mangosd revision.h)

#-----------------------------------------------------------------------------
# Append compile flags - if set by build definition
set(BUILD_PROPERTIES "-DHAVE_CONFIG_H")

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(BUILD_PROPERTIES "${BUILD_PROPERTIES} -DMANGOS_DEBUG")
endif()

set_target_properties(mangosd PROPERTIES COMPILE_FLAGS ${BUILD_PROPERTIES})

if(WIN32 AND WITH_MZ_SCRIPTS)
    set_target_properties(mangosd
    PROPERTIES
    ENABLE_EXPORTS ON
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}
    )
endif()

#-----------------------------------------------------------------------------
# Install the mangosd / mods configuration
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mangosd.conf.dist DESTINATION ${CONF_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mods.conf.dist DESTINATION ${CONF_DIR})
