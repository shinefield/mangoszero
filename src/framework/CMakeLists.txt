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
# Define the framework library
set(sources
    Common.cpp

    configuration/Config.cpp

    cryptography/AuthCrypt.cpp
    cryptography/BigNumber.cpp
    cryptography/Hmac.cpp
    cryptography/md5.c
    cryptography/Sha1.cpp

    database/Database.cpp
    database/DBCFileLoader.cpp
    database/Field.cpp
    database/SqlDelayThread.cpp
    database/SqlOperations.cpp
    database/SqlPreparedStatement.cpp
    database/SQLStorage.cpp

    log/Log.cpp

    network/ByteBuffer.cpp

    policies/ObjectLifeTime.cpp

    system/ProgressBar.cpp

    threading/Threading.cpp

    utilities/EventProcessor.cpp
    utilities/Util.cpp
)
set(headers
    Common.h
    revision_nr.h
    revision_sql.h

    configuration/Config.h

    cryptography/AuthCrypt.h
    cryptography/BigNumber.h
    cryptography/Hmac.h
    cryptography/md5.h
    cryptography/Sha1.h

    database/Database.h
    database/DatabaseEnv.h
    database/DatabaseImpl.h
    database/DBCFileLoader.h
    database/DBCStore.h
    database/Field.h
    database/QueryResult.h
    database/SqlDelayThread.h
    database/SqlOperations.h
    database/SqlPreparedStatement.h
    database/SQLStorage.h
    database/SQLStorageImpl.h

    debugging/Errors.h

    dynamic/FactoryHolder.h
    dynamic/ObjectRegistry.h

    game_system/Grid.h
    game_system/GridLoader.h
    game_system/GridReference.h
    game_system/GridRefManager.h
    game_system/NGrid.h
    game_system/TypeContainer.h
    game_system/TypeContainerFunctions.h
    game_system/TypeContainerVisitor.h

    log/Log.h

    network/ByteBuffer.h
    network/WorldPacket.h

    platform/CompilerDefs.h
    platform/Define.h

    policies/CreationPolicy.h
    policies/ObjectLifeTime.h
    policies/Singleton.h
    policies/ThreadingModel.h

    system/ProgressBar.h

    threading/LockedQueue.h
    threading/Threading.h

    utilities/ByteConverter.h
    utilities/Callback.h
    utilities/EventProcessor.h
    utilities/LinkedList.h
    utilities/LinkedReference/Reference.h
    utilities/LinkedReference/RefManager.h
    utilities/Timer.h
    utilities/TypeList.h
    utilities/UnorderedMapSet.h
    utilities/Util.h
)

#-----------------------------------------------------------------------------
# Add platform specific sources
if(WIN32)
    list(APPEND sources
        system/ServiceWin32.cpp
     )
    list(APPEND headers
        system/ServiceWin32.h
    )
else()
    list(APPEND sources
        system/PosixDaemon.cpp
    )
    list(APPEND headers
        system/PosixDaemon.h
    )
endif()

#-----------------------------------------------------------------------------
# Add database library specific sources
if(WITH_MYSQL AND MYSQL_FOUND)
    include_directories(
        ${MYSQL_INCLUDE_DIR}
    )
    list(APPEND sources
        database/DatabaseMysql.cpp
        database/QueryResultMysql.cpp
    )
    list(APPEND headers
        database/DatabaseMysql.h
        database/QueryResultMysql.h
    )
endif()

set(framework_LIB_SRCS ${sources} ${headers})

mangos_source_group(${framework_LIB_SRCS})

include_directories(
    ${ACE_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/dep/include/mersennetwister/
    ${CMAKE_SOURCE_DIR}/dep/include/utf8cpp/
    ${CMAKE_SOURCE_DIR}/src/framework/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}
)

#-----------------------------------------------------------------------------
# Configure the framework library
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/system/SystemConfig.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/system/SystemConfig.h)

#-----------------------------------------------------------------------------
# Build the framework library
add_library(mangos-framework STATIC ${framework_LIB_SRCS})
target_link_libraries(mangos-framework ${ACE_LIBRARIES})

add_dependencies(mangos-framework revision.h)

#-----------------------------------------------------------------------------
# Append compile flags - if set by build definition
set(BUILD_PROPERTIES "-DHAVE_CONFIG_H")

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(BUILD_PROPERTIES "${BUILD_PROPERTIES} -DMANGOS_DEBUG")
endif()

if(WITH_MYSQL AND MYSQL_FOUND)
    set(BUILD_PROPERTIES "${BUILD_PROPERTIES} -DDO_MYSQL")
    target_link_libraries(mangos-framework ${MYSQL_LIBRARIES})
endif()

set_target_properties(mangos-framework PROPERTIES COMPILE_FLAGS ${BUILD_PROPERTIES})
