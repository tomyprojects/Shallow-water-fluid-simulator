#ifndef RENDERHEIGHTMAP_H
#define RENDERHEIGHTMAP_H

#include "heightmap.h"

class RenderHeightMap
{
    HeightMap * mheightmap;
    float mdx,mdz,mhmax;
public:
    RenderHeightMap(HeightMap & map,float dx, float dz, float hmax = 10);

    void points();
    void triangles();
};

#endif // RENDERHEIGHTMAP_H
