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

#-----------------------------------------------------------------------------
# Install the database installation files
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/create_mysql.sql DESTINATION ${DATA_DIR}/database)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/drop_mysql.sql DESTINATION ${DATA_DIR}/database)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/characters.sql DESTINATION ${DATA_DIR}/database)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/mangos.sql DESTINATION ${DATA_DIR}/database)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/realmd.sql DESTINATION ${DATA_DIR}/database)

#-----------------------------------------------------------------------------
# Install the database update files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/updates DESTINATION ${DATA_DIR}/database)
