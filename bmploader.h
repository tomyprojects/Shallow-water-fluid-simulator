#ifndef BMPLOADER_H
#define BMPLOADER_H

#include <fstream>

class BmpLoader
{
    std::ifstream mfile;
    int mwidth;
    int mheight;
    unsigned short mbitsperpixel;
    unsigned mimagesize;
    unsigned mcompression;
    char * mimagep;

    void readBmpFileHeader(int * offset);
    void readDIBHeader();
    void readData(int offset);
public:
    typedef unsigned int BYTE;
    typedef unsigned int PIXEL;

    BmpLoader(const char* path);
    ~BmpLoader();

    char * data(BYTE i);
    char * pixel(unsigned x, unsigned y);
    unsigned bpp();
    PIXEL width();
    PIXEL height();
    BYTE padding();
};

#endif // BMPLOADER_H
