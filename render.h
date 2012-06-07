#ifndef RENDER_H
#define RENDER_H

#include "shallowwater.h"

template <class T>
class Render
{
    T * mheightmap;
    float mdx,mdz,mhmax;
public:
    Render(T & map,float dx, float dz, float hmax = 10);

    void points();
    void triangles();
};

#endif // RENDER_H
