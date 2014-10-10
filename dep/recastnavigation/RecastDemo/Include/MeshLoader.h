#ifndef MESHLOADER
#define MESHLOADER

class MeshLoader
{
public:
    //MeshLoader();

    virtual bool load(const char* fileName) = 0;

    virtual inline const float* getVerts() const = 0;
    virtual inline const float* getNormals() const = 0;
    virtual inline const int* getTris() const = 0;
    virtual inline int getVertCount() const = 0;
    virtual inline int getTriCount() const = 0;
    virtual inline const char* getFileName() const { return m_filename; }

protected:
    char m_filename[260];
};

#endif // MESHLOADER
