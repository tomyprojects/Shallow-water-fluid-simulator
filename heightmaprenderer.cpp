#include "heightmaprenderer.h"
#include <QtOpenGL>

HeightMapRenderer::HeightMapRenderer(HeightMap & map)
    : mheightmap(&map)
{

}

void HeightMapRenderer::points(float dx, float dz, float hmax)
{
    glBegin(GL_POINTS);

    for (int i=0; i<mheightmap->width(); i++)
        for (int j=0; j<mheightmap->height(); j++)
        {
            MapCell mc = (*mheightmap)(i,j);
            glColor3f(mc.r, mc.g, mc.b);
            glVertex3f(i * dx, hmax * mc.h , j * dz);
        }

    glEnd();
}
