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

#include <stdio.h>

#include "mpq_libmpq.h"
#include "loadlib.h"

class MPQFile;

FileLoader::FileLoader()
{
    data = 0;
    data_size = 0;
    version = 0;
}

FileLoader::~FileLoader()
{
    free();
}

bool FileLoader::loadFile(char* filename, bool log)
{
    free();
    MPQFile mf(filename);
    if (mf.isEof())
    {
        if (log)
            printf("No such file %s\n", filename);
        return false;
    }

    data_size = mf.getSize();

    data = new uint8 [data_size];
    if (data)
    {
        mf.read(data, data_size);
        mf.close();
        if (prepareLoadedData())
            return true;
    }
    printf("Error loading %s", filename);
    mf.close();
    free();
    return false;
}

bool FileLoader::prepareLoadedData()
{
    // Check version
    version = (file_MVER*) data;
    if (version->fcc != 'MVER')
        return false;
    if (version->ver != FILE_FORMAT_VERSION)
        return false;
    return true;
}

void FileLoader::free()
{
    if (data) delete[] data;
    data = 0;
    data_size = 0;
    version = 0;
}
