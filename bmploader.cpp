#include "bmploader.h"
#include "exception.h"
#include <cassert>

using std::ios;

void BmpLoader::readBmpFileHeader(int * offset)
{
    char bm[2];
    mfile.read(bm, 2);
    assert(bm[0] == 'B' && bm[1] == 'M');
    mfile.seekg(0xA,ios::beg);
    mfile.read((char*)offset, 4);
    assert(offset > 0);
}

void BmpLoader::readDIBHeader()
{
    mfile.seekg(0x12,ios::beg);
    mfile.read((char*)&mwidth, 4);
    assert(mwidth > 0);
    mfile.seekg(0x16, ios::beg);
    mfile.read((char*)&mheight, 4);
    assert(mheight != 0);
    mfile.seekg(0x1C, ios::beg);
    mfile.read((char*)&mbitsperpixel, 2);
    assert(mbitsperpixel == 8 ||mbitsperpixel == 16 || mbitsperpixel == 24 || mbitsperpixel == 32);
    mfile.seekg(0x1E, ios::beg);
    mfile.read((char*)&mcompression, 4);
    mfile.seekg(0x22, ios::beg);
    mfile.read((char*)&mimagesize, 4);

    if (mimagesize == 0)
        mimagesize = (unsigned int)(mwidth * mbitsperpixel / 32.0 + 1.0) * 4 * height();
}

void BmpLoader::readData(int offset)
{
    mimagep = new char[mimagesize];

    mfile.seekg(offset, ios::beg);
    mfile.read(mimagep, mimagesize);
}

BmpLoader::BmpLoader(const char* path)
    : mimagep(0)
{
    int offset;

    mfile.open(path, ios::in | ios::binary);

    throwif(!mfile.is_open());

    assert(mfile.good());

    readBmpFileHeader(&offset);

    readDIBHeader();

    readData(offset);

    mfile.close();
}

BmpLoader::~BmpLoader()
{
    if (mimagep) delete [] mimagep;
}

char * BmpLoader::data(BmpLoader::BYTE i)
{
    throwif (i > mimagesize);
    return mimagep + i;
}

char * BmpLoader::pixel(unsigned x, unsigned y)
{
    if (mheight < 0)
        return data(y * mimagesize / height() + x * mbitsperpixel / 8);
    data((height() - 1 - y) * mimagesize / height() + x * mbitsperpixel / 8);
}
unsigned BmpLoader::bpp() { return mbitsperpixel; }
BmpLoader::PIXEL BmpLoader::width() { return mwidth; }
BmpLoader::PIXEL BmpLoader::height() { return (mheight > 0 ? mheight : -mheight); }
BmpLoader::BYTE BmpLoader::padding() { return mimagesize / height() - mwidth * mbitsperpixel / 8; }
