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
# Define the Detour library
file(GLOB sources Source/*.cpp)
file(GLOB headers Include/*.h)

set(Detour_LIB_SRCS ${sources} ${headers})

mangos_source_group(${Detour_LIB_SRCS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

#-----------------------------------------------------------------------------
# Build the Detour library
add_library(Detour STATIC ${Detour_LIB_SRCS})

#-----------------------------------------------------------------------------
# Append compile flags - if set by build definition
set(BUILD_PROPERTIES "-DDT_POLYREF64")

set_target_properties(Detour PROPERTIES COMPILE_FLAGS ${BUILD_PROPERTIES})
