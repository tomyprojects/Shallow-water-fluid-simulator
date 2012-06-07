#include "renderheightmap.h"
#include <QtOpenGL>

RenderHeightMap::RenderHeightMap(HeightMap & map, float dx, float dz, float hmax)
    : mheightmap(&map), mdx(dx), mdz(dz), mhmax(hmax)
{

}

void RenderHeightMap::points()
{
    glBegin(GL_POINTS);

    for (unsigned i=0; i<mheightmap->width(); i++)
        for (unsigned j=0; j<mheightmap->height(); j++)
        {
            MapCell mc = (*mheightmap)(i,j);
            glColor3ub(mc.r, mc.g, mc.b);
            glVertex3f(i * mdx, mhmax * mc.h , j * mdz);
        }

    glEnd();
}
void RenderHeightMap::triangles()
{
    for (unsigned int i=0; i<mheightmap->width() - 1; i++)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (unsigned int j=0; j<mheightmap->height(); j++)
        {
            MapCell mc = (*mheightmap)(i,j);
            MapCell mc2 = (*mheightmap)(i+1,j);

            glColor3ub(mc2.r,mc2.g,mc2.b);
            glVertex3f((i+1) * mdx, mhmax * mc2.h , j * mdz);

            glColor3ub(mc.r,mc.g,mc.b);
            glVertex3f(i * mdx, mhmax * mc.h , j * mdz);
        }
        glEnd();
    }
}
