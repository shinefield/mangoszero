/**
 * mangos-zero is a full featured server for World of Warcraft in its vanilla
 * version, supporting clients for patch 1.12.x.
 *
 * Copyright (C) 2005-2014  MaNGOS project  <http://getmangos.com>
 * Parts Copyright (C) 2013-2014  CMaNGOS project <http://cmangos.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * World of Warcraft, and all World of Warcraft or Warcraft art, images,
 * and lore are copyrighted by Blizzard Entertainment, Inc.
 */

#ifndef MANGOS_H_WDTFILE
#define MANGOS_H_WDTFILE

#include <string>
#include <stdlib.h>

#include "mpq_libmpq.h"
#include "wmo.h"

class ADTFile;

class WDTFile
{
    public:
        WDTFile(char* file_name, char* file_name1);
        ~WDTFile(void);
        bool init(char* map_id, unsigned int mapID);

        string* gWmoInstanceName;
        int gnWMO, nMaps;

        ADTFile* GetMap(int x, int z);

    private:
        MPQFile WDT;
        bool maps[64][64];
        string filename;
};

#endif
