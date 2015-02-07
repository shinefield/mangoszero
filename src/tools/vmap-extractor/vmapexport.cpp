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

#include <cstdio>
#include <iostream>
#include <vector>
#include <list>
#include <errno.h>

#if defined WIN32
#include <Windows.h>
#include <sys/stat.h>
#include <direct.h>
#define mkdir _mkdir
#else
#include <sys/stat.h>
#endif

#undef min
#undef max

//#pragma warning(disable : 4505)
//#pragma comment(lib, "Winmm.lib")

#include <map>

//From Extractor
#include "adtfile.h"
#include "wdtfile.h"
#include "dbcfile.h"
#include "wmo.h"
#include "mpq_libmpq.h"

#include "vmapexport.h"

//------------------------------------------------------------------------------
// Defines

#define MPQ_BLOCK_SIZE 0x1000

//-----------------------------------------------------------------------------

extern ArchiveSet gOpenArchives;

typedef struct
{
    char name[64];
    unsigned int id;
} map_id;

map_id* map_ids;
uint16* LiqType = 0;
uint32 map_count;
char output_path[128] = ".";
char input_path[1024] = ".";
bool hasInputPathParam = false;
bool preciseVectorData = false;
bool skipJunkMaps = true;

// Constants
//static const char * szWorkDirMaps = ".\\Maps";
const char* szWorkDirWmo = "./Buildings";
const char* szRawVMAPMagic = "VMAPz05";

// Local testing functions

bool FileExists(const char* file)
{
    if (FILE* n = fopen(file, "rb"))
    {
        fclose(n);
        return true;
    }
    return false;
}

void strToLower(char* str)
{
    while (*str)
    {
        *str = tolower(*str);
        ++str;
    }
}

void ReadLiquidTypeTableDBC()
{
    printf("\nReading liquid types from LiquidType.dbc ... ");
    DBCFile dbc("DBFilesClient\\LiquidType.dbc");
    if (!dbc.open())
    {
        printf("Fatal error: invalid LiquidType.dbc file!\n");
        exit(1);
    }

    size_t LiqType_count = dbc.getRecordCount();
    size_t LiqType_maxid = dbc.getRecord(LiqType_count - 1).getUInt(0);
    LiqType = new uint16[LiqType_maxid + 1];
    memset(LiqType, 0xff, (LiqType_maxid + 1) * sizeof(uint16));

    for (uint32 x = 0; x < LiqType_count; ++x)
        LiqType[dbc.getRecord(x).getUInt(0)] = dbc.getRecord(x).getUInt(3);

    printf("%u liquid types loaded.\n", (unsigned int)LiqType_count);
}

bool shouldSkipMap(uint32 mapID)
{
    if (skipJunkMaps)
    {
        switch (mapID)
        {
            case 13:    // test.wdt
            case 25:    // ScottTest.wdt
            case 29:    // Test.wdt
            case 35:    // StormwindPrison.wdt
            case 37:    // PVPZone02.wdt
            case 42:    // Colin.wdt
            case 44:    // Monastery.wdt
            case 169:   // EmeraldDream.wdt (unused, and very large)
            case 451:   // development.wdt
                return true;
            default:
                break;
        }
    }
    return false;
}

bool ExtractWmo()
{
    printf("Extracting WMO files ... ");
    bool success = true;

    for (ArchiveSet::const_iterator ar_itr = gOpenArchives.begin(); ar_itr != gOpenArchives.end() && success; ++ar_itr)
    {
        vector<string> filelist;

        (*ar_itr)->GetFileListTo(filelist);
        for (vector<string>::iterator fname = filelist.begin(); fname != filelist.end() && success; ++fname)
        {
            if (fname->find(".wmo") != string::npos)
                success = ExtractSingleWmo(*fname);
        }
    }

    if (success)
        printf("done.\n");

    return success;
}

bool ExtractSingleWmo(std::string& fname)
{
    // Copy files from archive
    char szLocalFile[1024];
    const char* plain_name = GetPlainName(fname.c_str());
    sprintf(szLocalFile, "%s/%s", szWorkDirWmo, plain_name);
    fixnamen(szLocalFile, strlen(szLocalFile));

    if (FileExists(szLocalFile))
        return true;

    //Select root WMO files
    int p = 0;
    const char* rchr = strrchr(plain_name, '_');
    if (rchr != NULL)
    {
        char cpy[4];
        strncpy((char*)cpy, rchr, 4);
        for (int i = 0; i < 4; ++i)
        {
            int m = cpy[i];
            if (isdigit(m))
                p++;
        }
    }

    if (p == 3)
        return true;

    bool file_ok = true;
    // std::cout << "Extracting " << fname << std::endl;
    WMORoot froot(fname);

    if (!froot.open())
    {
        // printf("Couldn't open root WMO.\n");
        return true;
    }

    FILE* output = fopen(szLocalFile, "wb");
    if (!output)
    {
        printf("Fatal error: couldn't open %s for writing.\n", szLocalFile);
        return false;
    }

    froot.ConvertToVMAPRootWmo(output);
    int Wmo_nVertices = 0;

    if (froot.nGroups != 0)
    {
        // printf("Root has %d groups.\n", froot->nGroups);
        for (uint32 i = 0; i < froot.nGroups; ++i)
        {
            char temp[1024];
            strcpy(temp, fname.c_str());
            temp[fname.length() - 4] = 0;
            char groupFileName[1024];
            sprintf(groupFileName, "%s_%03d.wmo", temp, i);
            // printf("Trying to open group file %s\n",groupFileName);

            string s = groupFileName;
            WMOGroup fgroup(s);
            if (!fgroup.open())
            {
                printf("Fatal error: couldn't open all group files for: %s\n", plain_name);
                file_ok = false;
                break;
            }

            Wmo_nVertices += fgroup.ConvertToVMAPGroupWmo(output, &froot, preciseVectorData);
        }
    }

    fseek(output, 8, SEEK_SET); // store the correct no of vertices
    fwrite(&Wmo_nVertices, sizeof(int), 1, output);
    fclose(output);

    // Delete the extracted file in the case of an error
    if (!file_ok)
        remove(szLocalFile);
    return true;
}

void ParseMapFiles()
{
    printf("\nConverting map files ...\n");
    char fn[512];
    char id[10];
    StringSet failedPaths;
    //char id_filename[64];

    for (unsigned int i = 0; i < map_count; ++i)
    {
        sprintf(id, "%03u", map_ids[i].id);
        sprintf(fn, "World\\Maps\\%s\\%s.wdt", map_ids[i].name, map_ids[i].name);
        WDTFile WDT(fn, map_ids[i].name);

        if (WDT.init(id, map_ids[i].id))
        {
            if (shouldSkipMap(map_ids[i].id))
            {
                printf("Skipping   map %u - %s (%d/%d)\n", map_ids[i].id, map_ids[i].name, i + 1, map_count);
                continue;
            }

            printf("Extracting map %u - %s (%d/%d) \n", map_ids[i].id, map_ids[i].name, i + 1, map_count);
            for (int x = 0; x < 64; ++x)
            {
                for (int y = 0; y < 64; ++y)
                {
                    if (ADTFile* ADT = WDT.GetMap(x, y))
                    {
                        // sprintf(id_filename,"%02u %02u %03u",x,y,map_ids[i].id);
                        ADT->init(map_ids[i].id, x, y, failedPaths);
                        delete ADT;
                    }
                }
                // draw progress bar
                printf("Processing .......... %d%%\r", (100 * (x + 1)) / 64);
            }
        }
    }

    if (!failedPaths.empty())
    {
        printf("\nWarning: some models could not be extracted, see below.\n");
        for (StringSet::const_iterator itr = failedPaths.begin(); itr != failedPaths.end(); ++itr)
            printf("Couldn't find model %s\n", itr->c_str());
    }
}

bool scan_patches(char* scanmatch, std::vector<std::string>& pArchiveNames)
{
    int i;
    char path[512];

    for (i = 1; i <= 99; i++)
    {
        if (i != 1)
        {
            sprintf(path, "%s-%d.MPQ", scanmatch, i);
        }
        else
        {
            sprintf(path, "%s.MPQ", scanmatch);
        }
#ifdef __linux__
        if (FILE* h = fopen64(path, "rb"))
#else
        if (FILE* h = fopen(path, "rb"))
#endif
        {
            fclose(h);
            //matches.push_back(path);
            pArchiveNames.push_back(path);
        }
    }

    return (true);
}

bool fillArchiveNameVector(std::vector<std::string>& pArchiveNames)
{
    // printf("Game path: %s\n", input_path);
    char path[512];

    // open common files
    sprintf(path, "%sData/terrain.MPQ", input_path);
    pArchiveNames.push_back(path);
    sprintf(path, "%sData/model.MPQ", input_path);
    pArchiveNames.push_back(path);
    pArchiveNames.push_back(path);
    sprintf(path, "%sData/texture.MPQ", input_path);
    pArchiveNames.push_back(path);
    sprintf(path, "%sData/wmo.MPQ", input_path);
    pArchiveNames.push_back(path);
    sprintf(path, "%sData/base.MPQ", input_path);
    pArchiveNames.push_back(path);
    sprintf(path, "%sData/misc.MPQ", input_path);

    // now, scan for the patch levels
    sprintf(path, "%sData/patch", input_path);
    if (!scan_patches(path, pArchiveNames))
        return (false);

    printf("\n");

    return true;
}

void Usage(char* prg)
{
    printf("Usage: %s [OPTIONS]\n\n", prg);
    printf("Extract client database fiels and generate map files.\n");
    printf("   -h, --help                  show the usage\n");
    printf("   -i, --input <path>          search path for game client archives\n");
    printf("   -s, --small                 extract smaller vmaps by optimizing data. Reduces\n");
    printf("                               size by ~ 500MB\n");
    printf("   -l, --large                 extract larger vmaps with full data. Increases\n");
    printf("                               size by ~ 500MB\n");
    printf("   --skipJunkMaps [true|false] skip unused junk maps.\n");
    printf("\n");
    printf("Example:\n");
    printf("- use data path and create larger vmaps:\n");
    printf("  %s -l -i \"c:\\games\\world of warcraft\"\n", prg);
}

bool processArgv(int argc, char** argv)
{
    bool result = true;
    bool hasInputPathParam = false;
    bool preciseVectorData = false;
    char* param = NULL;

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            result = false;
            break;
        }
        else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--small") == 0)
        {
            result = true;
            preciseVectorData = false;
        }
        else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--large") == 0)
        {
            result = true;
            preciseVectorData = true;
        }
        else if (strcmp(argv[i], "--skipJunkMaps") == 0)
        {
            param = argv[++i];
            if (!param)
                result = false;

            result = true;

            if (strcmp(param, "true") == 0)
                skipJunkMaps = true;
            else if (strcmp(param, "false") == 0)
                skipJunkMaps = false;
            else
                printf("invalid option for '--skipJunkMaps', using default\n");
        }
        else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0)
        {
            param = argv[++i];
            if (!param)
            {
                result = false;
                break;
            }

            result = true;
            hasInputPathParam = true;
            strcpy(input_path, param);

            if (input_path[strlen(input_path) - 1] != '\\' || input_path[strlen(input_path) - 1] != '/')
            {
                strcat(input_path, "/");
            }
        }
        else
        {
            result = false;
            break;
        }
    }

    if (!result)
    {
        Usage(argv[0]);
    }
    return result;
}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Main
//
// The program must be run with two command line arguments
//
// Arg1 - The source MPQ name (for testing reading and file find)
// Arg2 - Listfile name
//

int main(int argc, char** argv)
{
    printf("mangos-zero vmap (version %s) extractor\n", szRawVMAPMagic);
    bool success = true;

    // Use command line arguments, when some
    if (!processArgv(argc, argv))
        return 1;

    // some simple check if working dir is dirty
    else
    {
        std::string sdir = std::string(szWorkDirWmo) + "/dir";
        std::string sdir_bin = std::string(szWorkDirWmo) + "/dir_bin";
        struct stat status;
        if (!stat(sdir.c_str(), &status) || !stat(sdir_bin.c_str(), &status))
        {
            printf("Fatal error: 'Buildings' directory already exist. Remove first.\n");
            return 1;
        }
    }

    // Create the working directory
    if (mkdir(szWorkDirWmo
#ifndef WIN32
              , 0711
#endif
             ))
        success = (errno == EEXIST);

    // prepare archive name list
    std::vector<std::string> archiveNames;
    fillArchiveNameVector(archiveNames);
    for (size_t i = 0; i < archiveNames.size(); ++i)
    {
        MPQArchive* archive = new MPQArchive(archiveNames[i].c_str());
        if (!gOpenArchives.size() || gOpenArchives.front() != archive)
            delete archive;
    }

    if (gOpenArchives.empty())
    {
        printf("Fatal error: No MPQ archive found in given path '%s'. Use -i option with proper path.\n", input_path);
        return 1;
    }
    ReadLiquidTypeTableDBC();

    // extract data
    if (success)
        success = ExtractWmo();

    // Map.dbc
    if (success)
    {
        DBCFile* dbc = new DBCFile("DBFilesClient\\Map.dbc");
        if (!dbc->open())
        {
            delete dbc;
            printf("Fatal error: Map.dbc not found in data file.\n");
            return 1;
        }

        printf("Reading maps from Map.dbc ... ");
        map_count = dbc->getRecordCount();
        map_ids = new map_id[map_count];
        for (unsigned int x = 0; x < map_count; ++x)
        {
            map_ids[x].id = dbc->getRecord(x).getUInt(0);
            strcpy(map_ids[x].name, dbc->getRecord(x).getString(1));
            // printf("Map - %s\n", map_ids[x].name);
        }
        printf("%u maps loaded.\n", map_count);

        delete dbc;
        ParseMapFiles();
        delete [] map_ids;

        // nError = ERROR_SUCCESS;
        // Extract models, listed in GameObjectDisplayInfo.dbc
        ExtractGameobjectModels();
    }

    printf("\n");
    if (!success)
    {
        printf("Fatal error: Extraction incomplete.\n   Precise vector data=%d.\n", preciseVectorData);
        delete [] LiqType;
        return 1;
    }

    printf("Extraction complete.\n");
    delete [] LiqType;
    return 0;
}
