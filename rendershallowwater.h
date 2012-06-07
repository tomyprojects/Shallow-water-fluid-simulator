#ifndef RENDERSHALLOWWATER_H
#define RENDERSHALLOWWATER_H

#include "shallowwater.h"
#include <QtOpenGL>

class RenderShallowWater
{
    typedef ShallowWaterBase::sw_float sw_float;
    ShallowWaterBase * mswp;
    sw_float mdx, mdy, mdz;
public:
    RenderShallowWater(ShallowWaterBase & sw,sw_float dx, sw_float dy, sw_float dz)
        : mswp(&sw), mdx(dx), mdy(dy), mdz(dz)
    {
    }

    void points()
    {
        glBegin(GL_POINTS);

        for (unsigned i=0; i<mswp->width(); i++)
            for (unsigned j=0; j<mswp->height(); j++)
            {
                glVertex3f(i * mdx, mdy * mswp->bottom(i,j) , j * mdz);
            }

        glEnd();
    }

    void lines(float magnify)
    {
        for (unsigned i=0; i<mswp->width() - 1; i++)
        {
            glBegin(GL_LINES);
            for (unsigned j=0; j<mswp->height(); j++)
            {

                glColor3f(0,0,1);
                glVertex3f(i * mdx, mdy * (*mswp)(i,j) , j * mdz);
                glColor3f(1,0,0);
                glVertex3f((i + mswp->velocity(i, j)[0]*magnify) * mdx, mdy * (*mswp)(i,j) ,
                           (j + mswp->velocity(i, j)[1]*magnify) * mdz);
            }
            glEnd();
        }
    }
};

#endif // RENDERSHALLOWWATER_H
