#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#define _USE_MATH_DEFINES
#include <math.h>

#include "MeshLoaderMap.h"
#include "Filelist.h"
#include "MapTree.h"
#include "Recast.h"

using namespace std;
using namespace MMAP;



MeshLoaderMap::MeshLoaderMap() : MeshLoader(), m_normals(NULL), m_lNormals(NULL)
{
}

MeshLoaderMap::~MeshLoaderMap()
{
    clear();
}

void MeshLoaderMap::clear()
{
    if (m_normals)
            delete[] m_normals;

    if (m_lNormals)
        delete[] m_lNormals;

    m_verts = NULL;
    m_tris = NULL;
    m_normals = NULL;
    m_lVerts = NULL;
    m_lTris = NULL;
    m_lNormals = NULL;
    m_lTriCount = 0;
    m_lVertCount = 0;
    m_triCount = 0;
    m_vertCount = 0;
}

float* MeshLoaderMap::computeNormales(float* verts, int* tris, int trisSize)
{
    float* normals = new float[trisSize*3];
    for (int i = 0; i < trisSize*3; i += 3)
    {
        const float* v0 = &verts[tris[i] * 3];
        const float* v1 = &verts[tris[i + 1] * 3];
        const float* v2 = &verts[tris[i + 2] * 3];
        float e0[3], e1[3];
        for (int j = 0; j < 3; ++j)
        {
            e0[j] = v1[j] - v0[j];
            e1[j] = v2[j] - v0[j];
        }
        float* n = &normals[i];
        n[0] = e0[1] * e1[2] - e0[2] * e1[1];
        n[1] = e0[2] * e1[0] - e0[0] * e1[2];
        n[2] = e0[0] * e1[1] - e0[1] * e1[0];
        float d = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
        if (d > 0)
        {
            d = 1.0f / d;
            n[0] *= d;
            n[1] *= d;
            n[2] *= d;
        }
    }
    return normals;
}

bool MeshLoaderMap::loadTile(unsigned int mapId, unsigned int tileX, unsigned int tileY, bool append)
{
    if (!append)
    {
        clear();
    }
    TerrainBuilder* terrainBuilder = new TerrainBuilder(false);

    terrainBuilder->loadMap(mapId, tileX, tileY, m_meshData);
    terrainBuilder->loadVMap(mapId, tileY, tileX, m_meshData);

    // get the coord bounds of the model data
    if (m_meshData.solidVerts.size() + m_meshData.liquidVerts.size() == 0)
    {
        clear();
        return false;
    }

    TerrainBuilder::cleanVertices(m_meshData.solidVerts, m_meshData.solidTris);
    TerrainBuilder::cleanVertices(m_meshData.liquidVerts, m_meshData.liquidTris);

    m_verts = m_meshData.solidVerts.getCArray();
    m_vertCount = m_meshData.solidVerts.size() / 3;
    m_tris = m_meshData.solidTris.getCArray();
    m_triCount = m_meshData.solidTris.size() / 3;

    m_lVerts = m_meshData.liquidVerts.getCArray();
    m_lVertCount = m_meshData.liquidVerts.size() / 3;
    m_lTris = m_meshData.liquidTris.getCArray();
    m_lTriCount = m_meshData.liquidTris.size() / 3;

    m_normals = computeNormales(m_verts, m_tris, m_triCount);
    m_lNormals = computeNormales(m_lVerts, m_lTris, m_lTriCount);
    return true;
}

bool MeshLoaderMap::load( const char* fileName )
{
    string fullname(fileName);
    int lastsep = fullname.find_last_of("/");
    string fname = fullname.substr(lastsep +1);
    string mapStr = fullname.substr(lastsep + 1, 3);
    unsigned int mapID = atoi(mapStr.c_str());

    strcpy(m_filename, fname.c_str());

    unsigned int tileY = unsigned int(atoi(fname.substr(3, 2).c_str()));
    unsigned int tileX = unsigned int(atoi(fname.substr(5, 2).c_str()));

    return loadTile(mapID, tileX, tileY);
}
