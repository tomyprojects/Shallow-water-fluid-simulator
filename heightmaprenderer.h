#ifndef HEIGHTMAPRENDERER_H
#define HEIGHTMAPRENDERER_H

#include "heightmap.h"

class HeightMapRenderer
{
    HeightMap * mheightmap;
public:
    HeightMapRenderer(HeightMap & map);

    void points(float dx, float dz, float hmax = 10);
};

#endif // HEIGHTMAPRENDERER_H
