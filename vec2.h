#ifndef VEC2_H
#define VEC2_H

template<class T=float>
class vec2
{
    T m[2];
public:
    vec2();
    vec2(T m0, T m1);

    T& x();
    T& y();

    vec2& operator= (const vec2 & v);

    T& operator[](int i);

    vec2& operator+= (const vec2 & v);
    vec2& operator-= (const vec2 & v);
    vec2& operator*= (T f);
    vec2& operator/= (T f);

    vec2 operator+ (vec2<T> v);
    vec2 operator- (vec2<T> v);
    vec2 operator* (T f);
    vec2 operator/ (T f);

    T squaredMagnitude();

    template<class U, class V>
    friend vec2<V> operator* (U f, const vec2<V> & v);
};

template<class T, class U>
vec2<U> operator* (T f, const vec2<U> & v) { return vec2<U>(v.m[0]*f, v.m[1]*f); }

#endif // VEC2_H
