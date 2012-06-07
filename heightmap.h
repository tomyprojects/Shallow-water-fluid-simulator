#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

struct MapCell
{
    unsigned char r, g, b;
    float h;
};

class HeightMap
{
    MapCell * mmap;
    unsigned int mwidth;
    unsigned int mheight;
    float mminheight;
public:
    HeightMap();
    HeightMap(const char* path);
    ~HeightMap();

    MapCell& operator() (unsigned i, unsigned j);
    void loadFromBmp(const char* path);

    unsigned int width();
    unsigned int height();
    float minHeight();
};

#endif // HEIGHTMAP_H
