#include "exception.h"

using namespace my;

Exception::Exception(const char * msg) :
        mmsg(msg)
{
}

const char* Exception::msg()
{
    return mmsg;
}

void my::throwExceptionIf(bool condition, const char * msg) { if (condition) throw Exception(msg); }

void my::throwMsg(const char *msg) { throw Exception(msg); }
