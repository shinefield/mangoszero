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

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string>
#include "SDL.h"
#include "SDL_opengl.h"
#include "imgui.h"
#include "InputGeom.h"
#include "CMaNGOS_Map.h"
#include "Recast.h"
#include "RecastDebugDraw.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourDebugDraw.h"
#include "NavMeshTesterTool.h"
#include "NavMeshPruneTool.h"
#include "OffMeshConnectionTool.h"
#include "ConvexVolumeTool.h"
#include "CrowdTool.h"
#include "MoveMapSharedDefines.h"

#ifdef WIN32
#  define snprintf _snprintf
#endif

inline unsigned int nextPow2(unsigned int v)
{
   v--;
   v |= v >> 1;
   v |= v >> 2;
   v |= v >> 4;
   v |= v >> 8;
   v |= v >> 16;
   v++;
   return v;
}

inline unsigned int ilog2(unsigned int v)
{
   unsigned int r;
   unsigned int shift;
   r = (v > 0xffff) << 4; v >>= r;
   shift = (v > 0xff) << 3; v >>= shift; r |= shift;
   shift = (v > 0xf) << 2; v >>= shift; r |= shift;
   shift = (v > 0x3) << 1; v >>= shift; r |= shift;
   r |= (v >> 1);
   return r;
}

class NavMeshTileTool : public SampleTool
{
   CMaNGOS_Map* m_sample;
   float m_hitPos[3];
   bool m_hitPosSet;
   float m_agentRadius;

public:

   NavMeshTileTool() :
       m_sample(0),
       m_hitPosSet(false),
       m_agentRadius(0)
   {
       m_hitPos[0] = m_hitPos[1] = m_hitPos[2] = 0;
   }

   virtual ~NavMeshTileTool()
   {
   }

   virtual int type() { return TOOL_TILE_EDIT; }

   virtual void init(Sample* sample)
   {
       m_sample = (CMaNGOS_Map*)sample;
   }

   virtual void reset() {}

   virtual void handleMenu()
   {
       imguiLabel("Create Tiles");
       if (imguiButton("Create All"))
       {
           /*if (m_sample)
               m_sample->buildAllTiles();*/
       }
       if (imguiButton("Remove All"))
       {
           /*if (m_sample)
               m_sample->removeAllTiles();*/
       }
   }

   virtual void handleClick(const float* /*s*/, const float*, bool)
   {
       /*m_hitPosSet = true;
       rcVcopy(m_hitPos,p);
       if (m_sample)
       {
           if (shift)
               m_sample->removeTile(m_hitPos);
           else
               m_sample->buildTile(m_hitPos);
       }*/
   }

   virtual void handleToggle() {}

   virtual void handleStep() {}

   virtual void handleUpdate(const float /*dt*/) {}

   virtual void handleRender()
   {
       if (m_hitPosSet)
       {
           const float s = m_sample->getAgentRadius();
           glColor4ub(0,0,0,128);
           glLineWidth(2.0f);
           glBegin(GL_LINES);
           glVertex3f(m_hitPos[0]-s,m_hitPos[1]+0.1f,m_hitPos[2]);
           glVertex3f(m_hitPos[0]+s,m_hitPos[1]+0.1f,m_hitPos[2]);
           glVertex3f(m_hitPos[0],m_hitPos[1]-s+0.1f,m_hitPos[2]);
           glVertex3f(m_hitPos[0],m_hitPos[1]+s+0.1f,m_hitPos[2]);
           glVertex3f(m_hitPos[0],m_hitPos[1]+0.1f,m_hitPos[2]-s);
           glVertex3f(m_hitPos[0],m_hitPos[1]+0.1f,m_hitPos[2]+s);
           glEnd();
           glLineWidth(1.0f);
       }
   }

   virtual void handleRenderOverlay(double* proj, double* model, int* view)
   {
       GLdouble x, y, z;
       if (m_hitPosSet && gluProject((GLdouble)m_hitPos[0], (GLdouble)m_hitPos[1], (GLdouble)m_hitPos[2],
                                     model, proj, view, &x, &y, &z))
       {
           int tx=0, ty=0;
           //m_sample->getTilePos(m_hitPos, tx, ty);
           char text[32];
           snprintf(text,32,"(%d,%d)", tx,ty);
           imguiDrawText((int)x, (int)y-25, IMGUI_ALIGN_CENTER, text, imguiRGBA(0,0,0,220));
       }

       // Tool help
       const int h = view[3];
       imguiDrawText(280, h-40, IMGUI_ALIGN_LEFT, "LMB: Rebuild hit tile.  Shift+LMB: Clear hit tile.", imguiRGBA(255,255,255,192));
   }
};

CMaNGOS_Map::CMaNGOS_Map() :
m_keepInterResults(false),
m_buildAll(true),
m_totalBuildTimeMs(0),
m_triareas(0),
m_solid(0),
m_chf(0),
m_cset(0),
m_pmesh(0),
m_dmesh(0),
m_mesh(0),
m_drawMode(DRAWMODE_NAVMESH),
m_maxTiles(0),
m_maxPolysPerTile(0),
m_tileSize(32),
m_tileCol(duRGBA(0, 0, 0, 32)),
m_tileBuildTime(0),
m_tileMemUsage(0),
m_tileTriCount(0),
m_isBuilded(false),
m_bigBaseUnit(false),
m_mapID(0),
m_showLevel(false),
m_drawLiquid(true),
m_drawMesh(true)
{
   resetCommonSettings();
   memset(m_tileBmin, 0, sizeof(m_tileBmin));
   memset(m_tileBmax, 0, sizeof(m_tileBmax));
    memset(m_tileFileName, 0, sizeof(m_tileFileName));

    m_mapIdSize = sizeof(g_map_ids) / sizeof(g_map_ids[0]);
}

CMaNGOS_Map::~CMaNGOS_Map()
{
    clear();
}

void CMaNGOS_Map::cleanup()
{
   delete [] m_triareas;
   m_triareas = 0;
   rcFreeHeightField(m_solid);
   m_solid = 0;
   rcFreeCompactHeightfield(m_chf);
   m_chf = 0;
   rcFreeContourSet(m_cset);
   m_cset = 0;
   rcFreePolyMesh(m_pmesh);
   m_pmesh = 0;
   rcFreePolyMeshDetail(m_dmesh);
   m_dmesh = 0;
}

void CMaNGOS_Map::clear()
{
    cleanup();
    dtFreeNavMesh(m_navMesh);
    m_navMesh = NULL;
    setTool(NULL);


    resetCommonSettings();
    m_isBuilded = false;
}

bool CMaNGOS_Map::loadMMapTile(int mapId, int tx, int ty)
{
    char tileFilename[256];
    char mmapFilename[256];
    snprintf(mmapFilename, sizeof(mmapFilename), "mmaps/%03d.mmap", mapId);
    snprintf(tileFilename, sizeof(tileFilename), "mmaps/%03d%02d%02d.mmtile", mapId, ty, tx);

    FILE* fp = fopen(mmapFilename, "rb");
    if (!fp)
    {
        m_ctx->log(RC_LOG_ERROR, "LoadMMapTile: '%s' not found!", mmapFilename);
        return false;
    }

    dtNavMeshParams params;
    fread(&params, sizeof(dtNavMeshParams), 1, fp);
    fclose(fp);
    m_navMesh = dtAllocNavMesh();
    dtStatus dtResult = m_navMesh->init(&params);
    if (dtStatusFailed(dtResult))
    {
        m_ctx->log(RC_LOG_ERROR, "LoadMMapTile: NavMesh init failed!");
        return false;
    }

    fp = fopen(tileFilename, "rb");
    if (!fp)
    {
        m_ctx->log(RC_LOG_ERROR, "LoadMMapTile: '%s' not found!", tileFilename);
        return false;
    }

    // Read header.
    MmapTileHeader header;
    fread(&header, sizeof(MmapTileHeader), 1, fp);
    if (header.mmapVersion != MMAP_VERSION)
    {
        fclose(fp);
        m_ctx->log(RC_LOG_ERROR, "LoadMMapTile: Mmap version is not correct! Found(%u), expected(%u)!", header.mmapVersion, unsigned int(MMAP_VERSION));
        return false;
    }

    unsigned char* data = (unsigned char*)dtAlloc(header.size, DT_ALLOC_PERM);
    if (!data)
    {
        m_ctx->log(RC_LOG_ERROR, "LoadMMapTile: Memory allocation failed!");
        fclose(fp);
        return false;
    }

    size_t result = fread(data, header.size, 1, fp);
    if (!result)
    {
        m_ctx->log(RC_LOG_ERROR, "LoadMMapTile: Problem to read '%s'!", tileFilename);
        return false;
    }
    fclose(fp);

    dtTileRef tileRef = 0;
    dtStatus dtRes = m_navMesh->addTile(data, header.size, DT_TILE_FREE_DATA, 0, &tileRef);
    if (dtStatusFailed(dtRes))
    {
        m_ctx->log(RC_LOG_ERROR, "LoadMMapTile: add tile failed!");
        return false;
    }
    return true;
}

void CMaNGOS_Map::resetCommonSettings()
{
    float baseUnitDim = m_bigBaseUnit ? 0.533333f : 0.266666f;
    float vertexPerTile = m_bigBaseUnit ? 40.0f : 80.0f;
    m_cellSize = baseUnitDim;
    m_cellHeight = baseUnitDim;
    m_agentHeight = m_bigBaseUnit ? 3.0f : 6.0f;
    m_agentRadius = m_bigBaseUnit ? 1.0f : 2.0f;
    m_agentMaxClimb = m_bigBaseUnit ? 1.0f : 2.0f;
    m_agentMaxSlope = 50.0f;
    m_regionMinSize = 60;
    m_regionMergeSize = 50;
    m_partitionType = SAMPLE_PARTITION_WATERSHED;
    m_edgeMaxLen = vertexPerTile +1;
    m_edgeMaxError = 2.0f;
    m_vertsPerPoly = 6.0f;
    m_detailSampleDist = 64.0f;
    m_detailSampleMaxError =  2.0f;
    m_tileSize = vertexPerTile;

    m_drawMesh = true;
    m_drawLiquid = true;
}


bool CMaNGOS_Map::ShowLevel(int height, int width)
{
    bool mouseOverMenu = false;
    if (m_showLevel)
    {
        static int levelScroll = 0;
        if (imguiBeginScrollArea("Choose Level", width - 10 - 250 - 10 - 200, height - 10 - 450, 200, 450, &levelScroll))
            mouseOverMenu = true;

        for (unsigned int i = 0; i < m_mapIdSize; ++i)
        {
            char buff[5];
            memset(buff, 0, sizeof(buff));
            itoa(g_map_ids[i], buff, 10);

            if (imguiItem(buff))
            {
                m_showLevel = false;
                m_mapID = g_map_ids[i];
            }
        }
        imguiEndScrollArea();
    }
    return mouseOverMenu;
}

void CMaNGOS_Map::handleExtraSettings()
{
    imguiLabel("Map Id");
    char buff[4];
    memset(buff, 0, sizeof(buff));
    itoa(m_mapID, buff, 10);
    if (imguiButton(buff))
    {
        m_showLevel = !m_showLevel;
    }
}

void CMaNGOS_Map::handleSettings()
{
    if (m_geom && m_geom->getMesh())
    {
        if (m_navMesh)
        {
            std::string fName;
            if (!m_isBuilded)
            {
                fName = "Unload ";
                fName += m_tileFileName;
            }
            else
            {
                fName = "Clear navmesh";
            }
            if (imguiButton(fName.c_str()))
            {
                clear();
            }
        }
        else
        {
            std::string fName = "Load ";
            fName += m_tileFileName;
            if (imguiButton(fName.c_str()))
            {
                loadMmap();
            }
        }
    }
    char tmpStr[50];
    imguiLabel("Rasterization");
    snprintf(tmpStr, sizeof(tmpStr), "Cell Size = %4.3f", m_cellSize);
    imguiValue(tmpStr);
    snprintf(tmpStr, sizeof(tmpStr), "Cell Height = %4.3f", m_cellHeight);
    imguiValue(tmpStr);

    if (m_geom)
    {
        const float* bmin = m_geom->getMeshBoundsMin();
        const float* bmax = m_geom->getMeshBoundsMax();
        int gw = 0, gh = 0;
        rcCalcGridSize(bmin, bmax, m_cellSize, &gw, &gh);
        char text[64];
        snprintf(text, 64, "Voxels  %d x %d", gw, gh);
        imguiValue(text);
    }

    if (m_mesh)
    {
        imguiSeparator();
        imguiLabel("Liquid stats");
        snprintf(tmpStr, sizeof(tmpStr), "Liquid> Verts: %.1fk  Tris: %.1fk",
            m_mesh->getLiqVertsCount() / 1000.0f,
            m_mesh->getLiqTrisCount() / 1000.0f);
        imguiValue(tmpStr);
    }

    imguiSeparator();
    imguiLabel("Agent");
    imguiSlider("Height", &m_agentHeight, 0.1f, 5.0f, 0.1f);
    imguiSlider("Radius", &m_agentRadius, 0.0f, 5.0f, 0.1f);
    imguiSlider("Max Climb", &m_agentMaxClimb, 0.1f, 5.0f, 0.1f);
    imguiSlider("Max Slope", &m_agentMaxSlope, 0.0f, 90.0f, 1.0f);

    imguiSeparator();
    imguiLabel("Region");
    imguiSlider("Min Region Size", &m_regionMinSize, 0.0f, 150.0f, 1.0f);
    imguiSlider("Merged Region Size", &m_regionMergeSize, 0.0f, 150.0f, 1.0f);

    imguiSeparator();
    imguiLabel("Partitioning");
    if (imguiCheck("Watershed", m_partitionType == SAMPLE_PARTITION_WATERSHED))
        m_partitionType = SAMPLE_PARTITION_WATERSHED;
    if (imguiCheck("Monotone", m_partitionType == SAMPLE_PARTITION_MONOTONE))
        m_partitionType = SAMPLE_PARTITION_MONOTONE;
    if (imguiCheck("Layers", m_partitionType == SAMPLE_PARTITION_LAYERS))
        m_partitionType = SAMPLE_PARTITION_LAYERS;

    imguiSeparator();
    imguiLabel("Polygonization");
    imguiSlider("Max Edge Length", &m_edgeMaxLen, 0.0f, 100.0f, 1.0f);
    imguiSlider("Max Edge Error", &m_edgeMaxError, 0.1f, 3.0f, 0.1f);
    imguiSlider("Verts Per Poly", &m_vertsPerPoly, 3.0f, 12.0f, 1.0f);

    imguiSeparator();
    imguiLabel("Detail Mesh");
    imguiSlider("Sample Distance", &m_detailSampleDist, 0.0f, 100.0f, 1.0f);
    imguiSlider("Max Sample Error", &m_detailSampleMaxError, 0.0f, 10.0f, 1.0f);

    imguiSeparator();

   if (imguiCheck("Keep Itermediate Results", m_keepInterResults))
       m_keepInterResults = !m_keepInterResults;

   if (imguiCheck("Build All Tiles", m_buildAll))
       m_buildAll = !m_buildAll;

   imguiLabel("Tiling");
   imguiSlider("TileSize", &m_tileSize, 16.0f, 1024.0f, 16.0f);

   if (m_geom)
   {
       const float* bmin = m_geom->getMeshBoundsMin();
       const float* bmax = m_geom->getMeshBoundsMax();
       char text[64];
       int gw = 0, gh = 0;
       rcCalcGridSize(bmin, bmax, m_cellSize, &gw, &gh);
       const int ts = (int)m_tileSize;
       const int tw = (gw + ts-1) / ts;
       const int th = (gh + ts-1) / ts;
       snprintf(text, 64, "Tiles  %d x %d", tw, th);
       imguiValue(text);

       // Max tiles and max polys affect how the tile IDs are caculated.
       // There are 22 bits available for identifying a tile and a polygon.
       int tileBits = rcMin((int)ilog2(nextPow2(tw*th)), 14);
       if (tileBits > 14) tileBits = 14;
       int polyBits = 22 - tileBits;
       m_maxTiles = 1 << tileBits;
       m_maxPolysPerTile = 1 << polyBits;
       snprintf(text, 64, "Max Tiles  %d", m_maxTiles);
       imguiValue(text);
       snprintf(text, 64, "Max Polys  %d", m_maxPolysPerTile);
       imguiValue(text);

   }
   else
   {
       m_maxTiles = 0;
       m_maxPolysPerTile = 0;
   }

   imguiSeparator();

   char msg[64];
   snprintf(msg, 64, "Build Time: %.1fms", m_totalBuildTimeMs);
   imguiLabel(msg);

   imguiSeparator();

   imguiSeparator();

}

void CMaNGOS_Map::handleTools()
{


   int type = !m_tool ? TOOL_NONE : m_tool->type();

    if (imguiCheck("Draw Mesh", m_drawMesh))
    {
        m_drawMesh = !m_drawMesh;
    }

    if (imguiCheck("Draw Liquid", m_drawLiquid))
    {
        m_drawLiquid = !m_drawLiquid;
    }

    if (!m_navMesh)
        return;

    imguiSeparatorLine();

   if (imguiCheck("Test Navmesh", type == TOOL_NAVMESH_TESTER))
   {
       setTool(new NavMeshTesterTool);
   }
   if (imguiCheck("Prune Navmesh", type == TOOL_NAVMESH_PRUNE))
   {
       setTool(new NavMeshPruneTool);
   }
   if (imguiCheck("Create Off-Mesh Links", type == TOOL_OFFMESH_CONNECTION))
   {
       setTool(new OffMeshConnectionTool);
   }
   if (imguiCheck("Create Convex Volumes", type == TOOL_CONVEX_VOLUME))
   {
       setTool(new ConvexVolumeTool);
   }
   if (imguiCheck("Create Crowds", type == TOOL_CROWD))
   {
       setTool(new CrowdTool);
   }

   imguiSeparatorLine();

   imguiIndent();

   if (m_tool)
       m_tool->handleMenu();

   imguiUnindent();
}

void CMaNGOS_Map::handleDebugMode()
{
   // Check which modes are valid.
   bool valid[MAX_DRAWMODE];
   for (int i = 0; i < MAX_DRAWMODE; ++i)
       valid[i] = false;

   if (m_geom)
   {
       valid[DRAWMODE_NAVMESH] = m_navMesh != 0;
       valid[DRAWMODE_NAVMESH_TRANS] = m_navMesh != 0;
       valid[DRAWMODE_NAVMESH_BVTREE] = m_navMesh != 0;
       valid[DRAWMODE_NAVMESH_NODES] = m_navQuery != 0;
       valid[DRAWMODE_NAVMESH_PORTALS] = m_navMesh != 0;
       valid[DRAWMODE_NAVMESH_INVIS] = m_navMesh != 0;
       valid[DRAWMODE_MESH] = true;
       valid[DRAWMODE_VOXELS] = m_solid != 0;
       valid[DRAWMODE_VOXELS_WALKABLE] = m_solid != 0;
       valid[DRAWMODE_COMPACT] = m_chf != 0;
       valid[DRAWMODE_COMPACT_DISTANCE] = m_chf != 0;
       valid[DRAWMODE_COMPACT_REGIONS] = m_chf != 0;
       valid[DRAWMODE_REGION_CONNECTIONS] = m_cset != 0;
       valid[DRAWMODE_RAW_CONTOURS] = m_cset != 0;
       valid[DRAWMODE_BOTH_CONTOURS] = m_cset != 0;
       valid[DRAWMODE_CONTOURS] = m_cset != 0;
       valid[DRAWMODE_POLYMESH] = m_pmesh != 0;
       valid[DRAWMODE_POLYMESH_DETAIL] = m_dmesh != 0;
   }

   int unavail = 0;
   for (int i = 0; i < MAX_DRAWMODE; ++i)
       if (!valid[i]) unavail++;

   if (unavail == MAX_DRAWMODE)
       return;

   imguiLabel("Draw");
   if (imguiCheck("Input Mesh", m_drawMode == DRAWMODE_MESH, valid[DRAWMODE_MESH]))
       m_drawMode = DRAWMODE_MESH;
   if (imguiCheck("Navmesh", m_drawMode == DRAWMODE_NAVMESH, valid[DRAWMODE_NAVMESH]))
       m_drawMode = DRAWMODE_NAVMESH;
   if (imguiCheck("Navmesh Invis", m_drawMode == DRAWMODE_NAVMESH_INVIS, valid[DRAWMODE_NAVMESH_INVIS]))
       m_drawMode = DRAWMODE_NAVMESH_INVIS;
   if (imguiCheck("Navmesh Trans", m_drawMode == DRAWMODE_NAVMESH_TRANS, valid[DRAWMODE_NAVMESH_TRANS]))
       m_drawMode = DRAWMODE_NAVMESH_TRANS;
   if (imguiCheck("Navmesh BVTree", m_drawMode == DRAWMODE_NAVMESH_BVTREE, valid[DRAWMODE_NAVMESH_BVTREE]))
       m_drawMode = DRAWMODE_NAVMESH_BVTREE;
   if (imguiCheck("Navmesh Nodes", m_drawMode == DRAWMODE_NAVMESH_NODES, valid[DRAWMODE_NAVMESH_NODES]))
       m_drawMode = DRAWMODE_NAVMESH_NODES;
   if (imguiCheck("Navmesh Portals", m_drawMode == DRAWMODE_NAVMESH_PORTALS, valid[DRAWMODE_NAVMESH_PORTALS]))
       m_drawMode = DRAWMODE_NAVMESH_PORTALS;
   if (imguiCheck("Voxels", m_drawMode == DRAWMODE_VOXELS, valid[DRAWMODE_VOXELS]))
       m_drawMode = DRAWMODE_VOXELS;
   if (imguiCheck("Walkable Voxels", m_drawMode == DRAWMODE_VOXELS_WALKABLE, valid[DRAWMODE_VOXELS_WALKABLE]))
       m_drawMode = DRAWMODE_VOXELS_WALKABLE;
   if (imguiCheck("Compact", m_drawMode == DRAWMODE_COMPACT, valid[DRAWMODE_COMPACT]))
       m_drawMode = DRAWMODE_COMPACT;
   if (imguiCheck("Compact Distance", m_drawMode == DRAWMODE_COMPACT_DISTANCE, valid[DRAWMODE_COMPACT_DISTANCE]))
       m_drawMode = DRAWMODE_COMPACT_DISTANCE;
   if (imguiCheck("Compact Regions", m_drawMode == DRAWMODE_COMPACT_REGIONS, valid[DRAWMODE_COMPACT_REGIONS]))
       m_drawMode = DRAWMODE_COMPACT_REGIONS;
   if (imguiCheck("Region Connections", m_drawMode == DRAWMODE_REGION_CONNECTIONS, valid[DRAWMODE_REGION_CONNECTIONS]))
       m_drawMode = DRAWMODE_REGION_CONNECTIONS;
   if (imguiCheck("Raw Contours", m_drawMode == DRAWMODE_RAW_CONTOURS, valid[DRAWMODE_RAW_CONTOURS]))
       m_drawMode = DRAWMODE_RAW_CONTOURS;
   if (imguiCheck("Both Contours", m_drawMode == DRAWMODE_BOTH_CONTOURS, valid[DRAWMODE_BOTH_CONTOURS]))
       m_drawMode = DRAWMODE_BOTH_CONTOURS;
   if (imguiCheck("Contours", m_drawMode == DRAWMODE_CONTOURS, valid[DRAWMODE_CONTOURS]))
       m_drawMode = DRAWMODE_CONTOURS;
   if (imguiCheck("Poly Mesh", m_drawMode == DRAWMODE_POLYMESH, valid[DRAWMODE_POLYMESH]))
       m_drawMode = DRAWMODE_POLYMESH;
   if (imguiCheck("Poly Mesh Detail", m_drawMode == DRAWMODE_POLYMESH_DETAIL, valid[DRAWMODE_POLYMESH_DETAIL]))
       m_drawMode = DRAWMODE_POLYMESH_DETAIL;

   if (unavail)
   {
       imguiValue("Tick 'Keep Itermediate Results'");
       imguiValue("rebuild some tiles to see");
       imguiValue("more debug mode options.");
   }
}

void CMaNGOS_Map::handleRender()
{
   if (!m_geom || !m_geom->getMesh())
       return;

   DebugDrawGL dd;

   const float texScale = 1.0f / (m_cellSize * 10.0f);

   // Draw mesh
   if (m_drawMesh && m_drawMode != DRAWMODE_NAVMESH_TRANS)
   {
       // Draw mesh
       duDebugDrawTriMeshSlope(&dd, m_geom->getMesh()->getVerts(), m_geom->getMesh()->getVertCount(),
                               m_geom->getMesh()->getTris(), m_geom->getMesh()->getNormals(), m_geom->getMesh()->getTriCount(),
                               m_agentMaxSlope, texScale);



       m_geom->drawOffMeshConnections(&dd);
   }

    if (m_drawLiquid)
    {
        duDebugDrawLiquidTriMesh(&dd, m_mesh->getLiqVerts(), m_mesh->getLiqVertsCount(),
            m_mesh->getLiqTris(), m_mesh->getLiqNormals(), m_mesh->getLiqTrisCount(), texScale);
    }

   glDepthMask(GL_FALSE);

   // Draw bounds
   const float* bmin = m_geom->getMeshBoundsMin();
   const float* bmax = m_geom->getMeshBoundsMax();
   duDebugDrawBoxWire(&dd, bmin[0],bmin[1],bmin[2], bmax[0],bmax[1],bmax[2], duRGBA(255,255,255,128), 1.0f);

   // Tiling grid.
   int gw = 0, gh = 0;
   rcCalcGridSize(bmin, bmax, m_cellSize, &gw, &gh);
   const int tw = (gw + (int)m_tileSize-1) / (int)m_tileSize;
   const int th = (gh + (int)m_tileSize-1) / (int)m_tileSize;
   const float s = m_tileSize*m_cellSize;
   duDebugDrawGridXZ(&dd, bmin[0],bmin[1],bmin[2], tw,th, s, duRGBA(0,0,0,64), 1.0f);

   // Draw active tile
   duDebugDrawBoxWire(&dd, m_tileBmin[0],m_tileBmin[1],m_tileBmin[2],
                      m_tileBmax[0],m_tileBmax[1],m_tileBmax[2], m_tileCol, 1.0f);

   if (m_navMesh && m_navQuery &&
       (m_drawMode == DRAWMODE_NAVMESH ||
        m_drawMode == DRAWMODE_NAVMESH_TRANS ||
        m_drawMode == DRAWMODE_NAVMESH_BVTREE ||
        m_drawMode == DRAWMODE_NAVMESH_NODES ||
        m_drawMode == DRAWMODE_NAVMESH_PORTALS ||
        m_drawMode == DRAWMODE_NAVMESH_INVIS))
   {
       if (m_drawMode != DRAWMODE_NAVMESH_INVIS)
           duDebugDrawNavMeshWithClosedList(&dd, *m_navMesh, *m_navQuery, m_navMeshDrawFlags);
       if (m_drawMode == DRAWMODE_NAVMESH_BVTREE)
           duDebugDrawNavMeshBVTree(&dd, *m_navMesh);
       if (m_drawMode == DRAWMODE_NAVMESH_PORTALS)
           duDebugDrawNavMeshPortals(&dd, *m_navMesh);
       if (m_drawMode == DRAWMODE_NAVMESH_NODES)
           duDebugDrawNavMeshNodes(&dd, *m_navQuery);
       duDebugDrawNavMeshPolysWithFlags(&dd, *m_navMesh, SAMPLE_POLYFLAGS_DISABLED, duRGBA(0,0,0,128));
   }


   glDepthMask(GL_TRUE);

   if (m_chf && m_drawMode == DRAWMODE_COMPACT)
       duDebugDrawCompactHeightfieldSolid(&dd, *m_chf);

   if (m_chf && m_drawMode == DRAWMODE_COMPACT_DISTANCE)
       duDebugDrawCompactHeightfieldDistance(&dd, *m_chf);
   if (m_chf && m_drawMode == DRAWMODE_COMPACT_REGIONS)
       duDebugDrawCompactHeightfieldRegions(&dd, *m_chf);
   if (m_solid && m_drawMode == DRAWMODE_VOXELS)
   {
       glEnable(GL_FOG);
       duDebugDrawHeightfieldSolid(&dd, *m_solid);
       glDisable(GL_FOG);
   }
   if (m_solid && m_drawMode == DRAWMODE_VOXELS_WALKABLE)
   {
       glEnable(GL_FOG);
       duDebugDrawHeightfieldWalkable(&dd, *m_solid);
       glDisable(GL_FOG);
   }

   if (m_cset && m_drawMode == DRAWMODE_RAW_CONTOURS)
   {
       glDepthMask(GL_FALSE);
       duDebugDrawRawContours(&dd, *m_cset);
       glDepthMask(GL_TRUE);
   }

   if (m_cset && m_drawMode == DRAWMODE_BOTH_CONTOURS)
   {
       glDepthMask(GL_FALSE);
       duDebugDrawRawContours(&dd, *m_cset, 0.5f);
       duDebugDrawContours(&dd, *m_cset);
       glDepthMask(GL_TRUE);
   }
   if (m_cset && m_drawMode == DRAWMODE_CONTOURS)
   {
       glDepthMask(GL_FALSE);
       duDebugDrawContours(&dd, *m_cset);
       glDepthMask(GL_TRUE);
   }
   if (m_chf && m_cset && m_drawMode == DRAWMODE_REGION_CONNECTIONS)
   {
       duDebugDrawCompactHeightfieldRegions(&dd, *m_chf);

       glDepthMask(GL_FALSE);
       duDebugDrawRegionConnections(&dd, *m_cset);
       glDepthMask(GL_TRUE);
   }
   if (m_pmesh && m_drawMode == DRAWMODE_POLYMESH)
   {
       glDepthMask(GL_FALSE);
       duDebugDrawPolyMesh(&dd, *m_pmesh);
       glDepthMask(GL_TRUE);
   }
   if (m_dmesh && m_drawMode == DRAWMODE_POLYMESH_DETAIL)
   {
       glDepthMask(GL_FALSE);
       duDebugDrawPolyMeshDetail(&dd, *m_dmesh);
       glDepthMask(GL_TRUE);
   }

   m_geom->drawConvexVolumes(&dd);

   if (m_tool)
       m_tool->handleRender();
   renderToolStates();

   glDepthMask(GL_TRUE);
}

void CMaNGOS_Map::handleRenderOverlay(double* proj, double* model, int* view)
{
   GLdouble x, y, z;

   // Draw start and end point labels
   if (m_tileBuildTime > 0.0f && gluProject((GLdouble)(m_tileBmin[0]+m_tileBmax[0])/2, (GLdouble)(m_tileBmin[1]+m_tileBmax[1])/2, (GLdouble)(m_tileBmin[2]+m_tileBmax[2])/2,
                                            model, proj, view, &x, &y, &z))
   {
       char text[32];
       snprintf(text,32,"%.3fms / %dTris / %.1fkB", m_tileBuildTime, m_tileTriCount, m_tileMemUsage);
       imguiDrawText((int)x, (int)y-25, IMGUI_ALIGN_CENTER, text, imguiRGBA(0,0,0,220));
   }

   if (m_tool)
       m_tool->handleRenderOverlay(proj, model, view);
   renderOverlayToolStates(proj, model, view);
}

void CMaNGOS_Map::handleMeshChanged(class InputGeom* geom)
{
   Sample::handleMeshChanged(geom);

    clear();
   resetToolStates();
   initToolStates(this);

    if (geom && geom->getMesh())
    {
        std::string fName = m_geom->getMesh()->getFileName();
        unsigned index = fName.find_last_of('.');
        if (index != std::string::npos)
            fName = fName.substr(0, index);
        strcpy(m_tileFileName, fName.c_str());
        strcat(m_tileFileName, ".mmap");

        std::string mapStr = fName.substr(0, 3);
        m_mapID = atoi(mapStr.c_str());

        m_tileX = unsigned int(atoi(fName.substr(3, 2).c_str()));
        m_tileY = unsigned int(atoi(fName.substr(5, 2).c_str()));

        m_mesh = (MeshLoaderMap const*)m_geom->getMesh();
    }
    else
    {
        memset(m_tileFileName, 0, sizeof(m_tileFileName));
        m_mesh = NULL;
    }
}

void CMaNGOS_Map::loadMmap()
{
    if (!m_geom || !m_geom->getMesh())
        return;

    clear();
    if (loadMMapTile(m_mapID, m_tileY, m_tileX))
    {
        m_navQuery->init(m_navMesh, 2048);
        setTool(new NavMeshTesterTool);
        m_isBuilded = false;
    }
    else
    {
        clear();
    }
}

bool CMaNGOS_Map::handleBuild()
{

    if (!m_geom || !m_geom->getMesh())
    {
        m_ctx->log(RC_LOG_ERROR, "buildNavigation: Input mesh is not specified.");
        return false;
    }

    cleanup();
    return buildMoveMapTile();

    /*if (m_cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
    {
        unsigned char* navData = 0;
        int navDataSize = 0;

        // Update poly flags from areas.
        for (int i = 0; i < m_pmesh->npolys; ++i)
        {
            if (m_pmesh->areas[i] == RC_WALKABLE_AREA)
                m_pmesh->areas[i] = SAMPLE_POLYAREA_GROUND;

            if (m_pmesh->areas[i] == SAMPLE_POLYAREA_GROUND ||
                m_pmesh->areas[i] == SAMPLE_POLYAREA_GRASS ||
                m_pmesh->areas[i] == SAMPLE_POLYAREA_ROAD)
            {
                m_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
            }
            else if (m_pmesh->areas[i] == SAMPLE_POLYAREA_WATER)
            {
                m_pmesh->flags[i] = SAMPLE_POLYFLAGS_SWIM;
            }
            else if (m_pmesh->areas[i] == SAMPLE_POLYAREA_DOOR)
            {
                m_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
            }
        }*/
}


bool CMaNGOS_Map::buildMoveMapTile()
{
    if (!m_geom || !m_geom->getMesh())
    {
        m_ctx->log(RC_LOG_ERROR, "buildNavigation: Input mesh is not specified.");
        return false;
    }

    const float* bmin = m_geom->getMeshBoundsMin();
    const float* bmax = m_geom->getMeshBoundsMax();
    const float* tVerts = m_geom->getMesh()->getVerts();
    const int tVertCount = m_geom->getMesh()->getVertCount();
    const int* tTris = m_geom->getMesh()->getTris();
    const int tTriCount = m_geom->getMesh()->getTriCount();

    const float* lVerts = m_mesh->getLiqVerts();
    const int lVertCount = m_mesh->getLiqVertsCount();
    const int* lTris = m_mesh->getLiqTris();
    const int lTriCount = m_mesh->getLiqTrisCount();
    const unsigned char* lTriFlags = m_mesh->getLiqFlags();
    memset(&m_cfg, 0, sizeof(rcConfig));

    rcVcopy(m_cfg.bmin, bmin);
    rcVcopy(m_cfg.bmax, bmax);

    // Reset build times gathering.
    m_ctx->resetTimers();

    // Start the build process.
    m_ctx->startTimer(RC_TIMER_TOTAL);

    m_cfg.cs = m_cellSize;
    m_cfg.ch = m_cellHeight;
    m_cfg.walkableSlopeAngle = m_agentMaxSlope;
    m_cfg.walkableHeight = (int)ceilf(m_agentHeight);// (int)ceilf(m_agentHeight / m_cfg.ch);
    m_cfg.walkableClimb = (int)floorf(m_agentMaxClimb);// (int)floorf(m_agentMaxClimb / m_cfg.ch);
    m_cfg.walkableRadius = (int)ceilf(m_agentRadius);// (int)ceilf(m_agentRadius / m_cfg.cs);
    m_cfg.maxEdgeLen = (int)m_edgeMaxLen;// (int)(m_edgeMaxLen / m_cellSize);
    m_cfg.maxSimplificationError = m_edgeMaxError;
    m_cfg.minRegionArea = (int)rcSqr(m_regionMinSize);     // Note: area = size*size
    m_cfg.mergeRegionArea = (int)rcSqr(m_regionMergeSize); // Note: area = size*size
    m_cfg.maxVertsPerPoly = (int)m_vertsPerPoly;
    m_cfg.tileSize = (int)m_tileSize;
    m_cfg.borderSize = m_cfg.walkableRadius + 3; // Reserve enough padding.
    m_cfg.detailSampleDist = m_cellSize * m_detailSampleDist;
    m_cfg.detailSampleMaxError = m_cellHeight * m_detailSampleMaxError;

    // this sets the dimensions of the heightfield - should maybe happen before border padding
    rcCalcGridSize(m_cfg.bmin, m_cfg.bmax, m_cfg.cs, &m_cfg.width, &m_cfg.height);

    m_ctx->log(RC_LOG_PROGRESS, "Building navigation:");
    m_ctx->log(RC_LOG_PROGRESS, " - %d x %d cells", m_cfg.width, m_cfg.height);
    m_ctx->log(RC_LOG_PROGRESS, " - %.1fK verts, %.1fK tris", tVertCount / 1000.0f, tTriCount / 1000.0f);

    m_ctx->log(RC_LOG_PROGRESS, "Config data:");
    m_ctx->log(RC_LOG_PROGRESS, "w: %i", m_cfg.width);
    m_ctx->log(RC_LOG_PROGRESS, "h: %i", m_cfg.height);
    m_ctx->log(RC_LOG_PROGRESS, "tilesize: %i", m_cfg.tileSize);
    m_ctx->log(RC_LOG_PROGRESS, "bordersize: %i", m_cfg.borderSize);
    m_ctx->log(RC_LOG_PROGRESS, "cs: %4.3f", m_cfg.cs);
    m_ctx->log(RC_LOG_PROGRESS, "ch: %4.3f", m_cfg.ch);
    m_ctx->log(RC_LOG_PROGRESS, "bmin: %4.4f, %4.4f, %4.4f", m_cfg.bmin[0], m_cfg.bmin[1], m_cfg.bmin[2]);
    m_ctx->log(RC_LOG_PROGRESS, "bmax: %4.4f, %4.4f, %4.4f", m_cfg.bmax[0], m_cfg.bmax[1], m_cfg.bmax[2]);
    m_ctx->log(RC_LOG_PROGRESS, "slope: %4.3f", m_cfg.walkableSlopeAngle);
    m_ctx->log(RC_LOG_PROGRESS, "walkHeight: %i", m_cfg.walkableHeight);
    m_ctx->log(RC_LOG_PROGRESS, "walkClimb: %i", m_cfg.walkableClimb);
    m_ctx->log(RC_LOG_PROGRESS, "walkRadius: %i", m_cfg.walkableRadius);
    m_ctx->log(RC_LOG_PROGRESS, "maxEdgeLen: %i", m_cfg.maxEdgeLen);
    m_ctx->log(RC_LOG_PROGRESS, "maxSimplError: %4.3f", m_cfg.maxSimplificationError);
    m_ctx->log(RC_LOG_PROGRESS, "minRegionArea: %i", m_cfg.minRegionArea);
    m_ctx->log(RC_LOG_PROGRESS, "mergeRegionArea: %i", m_cfg.mergeRegionArea);
    m_ctx->log(RC_LOG_PROGRESS, "vertPerPoly: %i", m_cfg.maxVertsPerPoly);
    m_ctx->log(RC_LOG_PROGRESS, "detailSampleDist: %4.3f", m_cfg.detailSampleDist);
    m_ctx->log(RC_LOG_PROGRESS, "detailSampleMaxError: %4.3f", m_cfg.detailSampleMaxError);

    // build all tiles
    m_solid = rcAllocHeightfield();
    if (!m_solid || !rcCreateHeightfield(m_ctx, *m_solid, m_cfg.width, m_cfg.height, m_cfg.bmin, m_cfg.bmax, m_cfg.cs, m_cfg.ch))
    {
        m_ctx->log(RC_LOG_ERROR, "Failed building heightfield!");
        return false;
    }

    // mark all walkable tiles, both liquids and solids
    unsigned char* triFlags = new unsigned char[tTriCount];
    memset(triFlags, NAV_GROUND, tTriCount * sizeof(unsigned char));
    rcClearUnwalkableTriangles(m_ctx, m_cfg.walkableSlopeAngle, tVerts, tVertCount, tTris, tTriCount, triFlags);
    rcRasterizeTriangles(m_ctx, tVerts, tVertCount, tTris, triFlags, tTriCount, *m_solid, m_cfg.walkableClimb);
    delete[] triFlags;

    rcFilterLowHangingWalkableObstacles(m_ctx, m_cfg.walkableClimb, *m_solid);
    rcFilterLedgeSpans(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid);
    rcFilterWalkableLowHeightSpans(m_ctx, m_cfg.walkableHeight, *m_solid);

    rcRasterizeTriangles(m_ctx, lVerts, lVertCount, lTris, lTriFlags, lTriCount, *m_solid, m_cfg.walkableClimb);

    // compact heightfield spans
    m_chf = rcAllocCompactHeightfield();
    if (!m_chf || !rcBuildCompactHeightfield(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid, *m_chf))
    {
        m_ctx->log(RC_LOG_ERROR, "Failed compacting heightfield!");
        return false;
    }

    // build polymesh intermediates
    if (!rcErodeWalkableArea(m_ctx, m_cfg.walkableRadius, *m_chf))
    {
        m_ctx->log(RC_LOG_ERROR, "Failed eroding area!");
        return false;
    }

    if (m_partitionType == SAMPLE_PARTITION_WATERSHED)
    {
        // Prepare for region partitioning, by calculating distance field along the walkable surface.
        if (!rcBuildDistanceField(m_ctx, *m_chf))
        {
            m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build distance field.");
            return false;
        }

        // Partition the walkable surface into simple regions without holes.
        if (!rcBuildRegions(m_ctx, *m_chf, m_cfg.borderSize, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
        {
            m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build watershed regions.");
            return false;
        }
    }
    else if (m_partitionType == SAMPLE_PARTITION_MONOTONE)
    {
        // Partition the walkable surface into simple regions without holes.
        // Monotone partitioning does not need distancefield.
        if (!rcBuildRegionsMonotone(m_ctx, *m_chf, m_cfg.borderSize, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
        {
            m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build monotone regions.");
            return false;
        }
    }
    else // SAMPLE_PARTITION_LAYERS
    {
        // Partition the walkable surface into simple regions without holes.
        if (!rcBuildLayerRegions(m_ctx, *m_chf, m_cfg.borderSize, m_cfg.minRegionArea))
        {
            m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build layer regions.");
            return false;
        }
    }

    if (!rcBuildRegions(m_ctx, *m_chf, m_cfg.borderSize, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
    {
        m_ctx->log(RC_LOG_ERROR, "Failed building regions!");
        return false;
    }

    m_cset = rcAllocContourSet();
    if (!m_cset || !rcBuildContours(m_ctx, *m_chf, m_cfg.maxSimplificationError, m_cfg.maxEdgeLen, *m_cset))
    {
        m_ctx->log(RC_LOG_ERROR, "Failed building contours!");
        return false;
    }

    // build polymesh
    m_pmesh = rcAllocPolyMesh();
    if (!m_pmesh || !rcBuildPolyMesh(m_ctx, *m_cset, m_cfg.maxVertsPerPoly, *m_pmesh))
    {
        m_ctx->log(RC_LOG_ERROR, "Failed building polymesh!");
        return false;
    }

    m_dmesh = rcAllocPolyMeshDetail();
    if (!m_dmesh || !rcBuildPolyMeshDetail(m_ctx, *m_pmesh, *m_chf, m_cfg.detailSampleDist, m_cfg.detailSampleMaxError, *m_dmesh))
    {
        m_ctx->log(RC_LOG_ERROR, "Failed building polymesh detail!");
        return false;
    }

    // free those up
    // we may want to keep them in the future for debug
    // but right now, we don't have the code to merge them
    rcFreeHeightField(m_solid);
    m_solid = NULL;
    rcFreeCompactHeightfield(m_chf);
    m_chf = NULL;
    rcFreeContourSet(m_cset);
    m_cset = NULL;

    // set polygons as walkable
    // TODO: special flags for DYNAMIC polygons, ie surfaces that can be turned on and off
    for (int i = 0; i < m_pmesh->npolys; ++i)
        if (m_pmesh->areas[i] & RC_WALKABLE_AREA)
            m_pmesh->flags[i] = m_pmesh->areas[i];

    // setup mesh parameters
    dtNavMeshCreateParams params;
    memset(&params, 0, sizeof(params));
    params.verts = m_pmesh->verts;
    params.vertCount = m_pmesh->nverts;
    params.polys = m_pmesh->polys;
    params.polyAreas = m_pmesh->areas;
    params.polyFlags = m_pmesh->flags;
    params.polyCount = m_pmesh->npolys;
    params.nvp = m_pmesh->nvp;
    params.detailMeshes = m_dmesh->meshes;
    params.detailVerts = m_dmesh->verts;
    params.detailVertsCount = m_dmesh->nverts;
    params.detailTris = m_dmesh->tris;
    params.detailTriCount = m_dmesh->ntris;

    params.offMeshConVerts = m_geom->getOffMeshConnectionVerts();
    params.offMeshConRad = m_geom->getOffMeshConnectionRads();
    params.offMeshConDir = m_geom->getOffMeshConnectionDirs();
    params.offMeshConAreas = m_geom->getOffMeshConnectionAreas();
    params.offMeshConFlags = m_geom->getOffMeshConnectionFlags();
    params.offMeshConUserID = m_geom->getOffMeshConnectionId();
    params.offMeshConCount = m_geom->getOffMeshConnectionCount();

    params.walkableHeight = m_cellHeight * m_cfg.walkableHeight;  // agent height
    params.walkableRadius = m_cellSize * m_cfg.walkableRadius;  // agent radius
    params.walkableClimb = m_cellHeight * m_cfg.walkableClimb;    // keep less that walkableHeight (aka agent height)!
    /*params.tileX = (((bmin[0] + bmax[0]) / 2) - m_navMesh->getParams()->orig[0]) / GRID_SIZE;
    params.tileY = (((bmin[2] + bmax[2]) / 2) - m_navMesh->getParams()->orig[2]) / GRID_SIZE;*/


    rcVcopy(params.bmin, m_pmesh->bmin);
    rcVcopy(params.bmax, m_pmesh->bmax);
    params.cs = m_cfg.cs;
    params.ch = m_cfg.ch;

    // no use of layer
    params.tileLayer = 0;
    params.buildBvTree = true;

    // will hold final navmesh
    unsigned char* navData = NULL;
    int navDataSize = 0;

        // these values are checked within dtCreateNavMeshData - handle them here
        // so we have a clear error message
        if (params.nvp > DT_VERTS_PER_POLYGON)
        {
            m_ctx->log(RC_LOG_ERROR, "Invalid verts-per-polygon value!");
            return false;
        }
        if (params.vertCount >= 0xffff)
        {
            m_ctx->log(RC_LOG_ERROR, "Too many vertices!");
            return false;
        }
        if (!params.vertCount || !params.verts)
        {
            // occurs mostly when adjacent tiles have models
            // loaded but those models don't span into this tile

            // message is an annoyance
            m_ctx->log(RC_LOG_ERROR, "No vertices to build!");
            return false;
        }
        if (!params.polyCount || !params.polys)
        {
            // we have flat tiles with no actual geometry - don't build those, its useless
            // keep in mind that we do output those into debug info
            // drop tiles with only exact count - some tiles may have geometry while having less tiles
            m_ctx->log(RC_LOG_ERROR, "No polygons to build on tile!");
            return false;
        }
        if (!params.detailMeshes || !params.detailVerts || !params.detailTris)
        {
            m_ctx->log(RC_LOG_ERROR, "No detail mesh to build!");
            return false;
        }

        //printf("%s Building navmesh tile...                \r", tileString);
        if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
        {
            m_ctx->log(RC_LOG_ERROR, "Failed building navmesh!");
            return false;
        }

        m_navMesh = dtAllocNavMesh();
        if (!m_navMesh)
        {
            dtFree(navData);
            m_ctx->log(RC_LOG_ERROR, "Could not create Detour navmesh");
            return false;
        }

        dtStatus status;

        status = m_navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
        if (dtStatusFailed(status))
        {
            dtFree(navData);
            m_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh");
            return false;
        }

        status = m_navQuery->init(m_navMesh, 2048);
        if (dtStatusFailed(status))
        {
            m_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh query");
            return false;
        }

        m_ctx->stopTimer(RC_TIMER_TOTAL);

        // Show performance stats.
        duLogBuildTimes(*m_ctx, m_ctx->getAccumulatedTime(RC_TIMER_TOTAL));
        m_ctx->log(RC_LOG_PROGRESS, ">> Polymesh: %d vertices  %d polygons", m_pmesh->nverts, m_pmesh->npolys);

        m_totalBuildTimeMs = m_ctx->getAccumulatedTime(RC_TIMER_TOTAL) / 1000.0f;

        if (m_tool)
            m_tool->init(this);
        initToolStates(this);
        m_isBuilded = true;

        return true;
}

const char* CMaNGOS_Map::getExtension()
{
    char buff[20];
    snprintf(buff, sizeof(buff), "%03d*.map", m_mapID);
    strcpy(m_searchMask, buff);
    return m_searchMask;
}
