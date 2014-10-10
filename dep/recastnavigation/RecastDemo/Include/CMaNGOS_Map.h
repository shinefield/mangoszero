//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#ifndef RECASTCMANGOSMAP_H
#define RECASTCMANGOSMAP_H

#include "Sample.h"
#include "DetourNavMesh.h"
#include "Recast.h"
#include "ChunkyTriMesh.h"
#include "MeshLoaderMap.h"

#ifdef WIN32
#  define snprintf _snprintf
#  define putenv _putenv
#endif

static const unsigned int g_map_ids[] =
{ 0, 1, 13, 25, 30, 33, 34, 35, 36, 37, 42, 43, 44, 47, 48, 70, 90, 109,
129, 169, 189, 209, 229, 230, 249, 269, 289, 309, 329, 349, 369, 389,
409, 429, 429, 449, 450, 451, 469, 489, 509, 529, 530, 531, 532, 533,
534, 540, 542, 543, 544, 545, 546, 547, 548, 550, 552, 553, 554, 555,
556, 557, 558, 559, 560, 562, 564, 565, 566, 568, 571, 572, 573, 574,
575, 576, 578, 580, 582, 584, 585, 586, 587, 588, 589, 590, 591, 592,
593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606,
607, 608, 609, 610, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621,
622, 623, 624, 628, 631, 632, 641, 642, 647, 649, 650, 658, 668, 672,
673, 712, 713, 718, 723, 724 };

class CMaNGOS_Map : public Sample
{
protected:
    MeshLoaderMap const* m_mesh;
   bool m_keepInterResults;
   bool m_buildAll;
   float m_totalBuildTimeMs;

   unsigned char* m_triareas;
   rcHeightfield* m_solid;
   rcCompactHeightfield* m_chf;
   rcContourSet* m_cset;
   rcPolyMesh* m_pmesh;
   rcPolyMeshDetail* m_dmesh;
   rcConfig m_cfg;

   enum DrawMode
   {
       DRAWMODE_NAVMESH,
       DRAWMODE_NAVMESH_TRANS,
       DRAWMODE_NAVMESH_BVTREE,
       DRAWMODE_NAVMESH_NODES,
       DRAWMODE_NAVMESH_PORTALS,
       DRAWMODE_NAVMESH_INVIS,
       DRAWMODE_MESH,
       DRAWMODE_VOXELS,
       DRAWMODE_VOXELS_WALKABLE,
       DRAWMODE_COMPACT,
       DRAWMODE_COMPACT_DISTANCE,
       DRAWMODE_COMPACT_REGIONS,
       DRAWMODE_REGION_CONNECTIONS,
       DRAWMODE_RAW_CONTOURS,
       DRAWMODE_BOTH_CONTOURS,
       DRAWMODE_CONTOURS,
       DRAWMODE_POLYMESH,
       DRAWMODE_POLYMESH_DETAIL,
       MAX_DRAWMODE
   };

   DrawMode m_drawMode;
    bool m_drawMesh;
    bool m_drawLiquid;

   int m_maxTiles;
   int m_maxPolysPerTile;
   float m_tileSize;

   unsigned int m_tileCol;
   float m_tileBmin[3];
   float m_tileBmax[3];
   float m_tileBuildTime;
   float m_tileMemUsage;
   int m_tileTriCount;

    char m_tileFileName[256];
    char m_searchMask[8];
    bool m_isBuilded;
    bool m_bigBaseUnit;

    unsigned int m_mapID;
    unsigned int m_tileX, m_tileY;
    bool m_showLevel;
    unsigned int m_mapIdSize;

   void cleanup();

   void saveAll(const char* path, const dtNavMesh* mesh);
   dtNavMesh* loadAll(const char* path);
   bool loadMMapTile(int mapId, int tx, int ty);
public:
   CMaNGOS_Map();
   virtual ~CMaNGOS_Map();

   virtual void handleSettings();
   virtual void handleTools();
   virtual void handleDebugMode();
   virtual void handleRender();
   virtual void handleRenderOverlay(double* proj, double* model, int* view);
   virtual void handleMeshChanged(class InputGeom* geom);
   virtual bool handleBuild();
    virtual bool ShowLevel(int height, int width);
    virtual SampleType getSampleType() { return CMANGOS_MAP_SAMPLE; };
    virtual const char* getFolder() { return "maps"; };
    virtual const char* getExtension();// { return "*.map"; };

    void loadMmap();
    void clear();
    void resetCommonSettings();
    bool buildMoveMapTile();
    void handleExtraSettings();
};

#endif // RECASTCMANGOSMAP_H
