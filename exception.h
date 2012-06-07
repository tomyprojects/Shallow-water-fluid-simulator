#ifndef MY_EXCEPTION_H
#define MY_EXCEPTION_H

#define throwif(condition) my::throwExceptionIf(condition, #condition)
#define throwIf(condition) my::throwExceptionIf(condition, #condition)

namespace my {
    class Exception
    {
        const char * mmsg;

    public:
        Exception(const char * msg);
        const char* msg();
    };

    void throwExceptionIf(bool condition, const char * msg);
    void throwMsg(const char* msg);
}

#endif
