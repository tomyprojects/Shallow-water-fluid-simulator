#ifndef TESTVEC2_H
#define TESTVEC2_H

#include "vec2.h"
#include <cassert>
#include <QtDebug>

typedef vec2<float> vec2f;

class TestVec2
{
public:
    TestVec2()
    {
        assert(test(vec2f(1,2)+vec2f(2,1),3,3));
        assert(test(vec2f(1,2)-vec2f(2,1),-1,1));
        assert(test(vec2f(1,2)*2,2,4));
        assert(test(2*vec2f(1,2),2,4));
        assert(test(vec2f(1,2)/2,0.5,1));

        vec2f v(1,1);
        vec2f v2(2,2);

        v += v2;
        assert(test(v,3,3));
        v -= v2;
        assert(test(v,1,1));
        v *= 3;
        assert(test(v,3,3));
        v /= 3;
        assert(test(v,1,1));
        v2 = v;
        assert(test(v2,1,1));
        assert(test(2*v2*2 + v2 + v,6,6));
    }

    bool test(vec2f v, float m0, float m1)
    {
        bool result = (v[0] == m0) && (v[1] == m1);

        if (!result)
        {
            qDebug() << "v[0] " << v[0] << " m0 " << m0;
            qDebug() << "v[1] " << v[1] << " m1 " << m1;
        }

        return result;
    }
};

#endif // TESTVEC2_H
