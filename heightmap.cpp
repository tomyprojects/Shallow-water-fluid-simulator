#include "heightmap.h"
#include "exception.h"
#include "bmploader.h"
#include <QtDebug>

HeightMap::HeightMap()
    : mmap(0), mwidth(0), mheight(0), mminheight(1.0)
{
}

HeightMap::HeightMap(const char* path)
    : mmap(0), mwidth(0), mheight(0), mminheight(1.0)
{
    loadFromBmp(path);
}

HeightMap::~HeightMap()
{
    if (mmap) delete [] mmap;
}

MapCell& HeightMap::operator() (unsigned i, unsigned j)
{
    unsigned offset = i + j * mwidth;

    throwif(i > mwidth || j > mheight);

    return mmap[offset];
}

void HeightMap::loadFromBmp(const char* path)
{
    BmpLoader loader(path);

    throwif(loader.bpp() != 24);

    mmap = new MapCell[loader.width() * loader.height()];

    mwidth = loader.width();

    mheight = loader.height();

    for (unsigned int i=0; i<loader.width(); i++)
        for (unsigned int j=0; j<loader.height(); j++)
        {
            (*this)(i,j).b = (loader.pixel(i, j))[0];
            (*this)(i,j).g = (loader.pixel(i, j) + 1)[0];
            (*this)(i,j).r = (loader.pixel(i, j) + 2)[0];
            (*this)(i,j).h = ( 0.3 * (*this)(i,j).r + 0.4 * (*this)(i,j).g + 0.3 * (*this)(i,j).b) / (255.0);
            if (mminheight > (*this)(i,j).h)
                mminheight = (*this)(i,j).h;
        }
}

float HeightMap::minHeight()
{
    return mminheight;
}

unsigned int HeightMap::width() { return mwidth; }
unsigned int HeightMap::height() { return mheight; }
