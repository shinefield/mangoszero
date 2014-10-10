#ifndef MESHLOADER_MAP
#define MESHLOADER_MAP

#include "MeshLoader.h"
#include <vector>

#include "Recast.h"
#include "TerrainBuilder.h"

class MeshLoaderMap : public MeshLoader
{
public:
    MeshLoaderMap();
    ~MeshLoaderMap();

    typedef std::vector<int> tIntVect;
    typedef std::vector<float> tFloatVect;

    virtual bool load(const char* fileName);

    virtual inline const float* getVerts() const { return m_verts; }
    virtual inline const float* getNormals() const { return m_normals; }
    virtual inline const int* getTris() const { return m_tris; }
    virtual inline int getVertCount() const { return m_vertCount; }
    virtual inline int getTriCount() const { return m_triCount; }

    inline const float* getLiqVerts() const { return m_lVerts; }
    inline int getLiqVertsCount() const { return m_lVertCount; }
    inline const int* getLiqTris() const { return m_lTris; }
    inline int getLiqTrisCount() const { return m_lTriCount; }
    inline unsigned char* getLiqFlags() const { return (unsigned char*)m_meshData.liquidType.getCArray(); }
    inline const float* getLiqNormals() const { return m_lNormals; }

    virtual inline const char* getFileName() const { return m_filename; }

private:
    bool loadTile(unsigned int mapId, unsigned int x, unsigned int y, bool append = false);
    float* computeNormales(float* verts, int* tris, int trisSize);
    void clear();

    MMAP::MeshData m_meshData;

    float* m_verts;
    int* m_tris;
    float* m_normals;
    int m_vertCount;
    int m_triCount;

    float* m_lVerts;
    int* m_lTris;
    float* m_lNormals;
    int m_lVertCount;
    int m_lTriCount;
};
#endif // MESHLOADER_MAP
