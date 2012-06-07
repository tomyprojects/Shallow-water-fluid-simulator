#ifndef VEC3_H
#define VEC3_H

template<class T=float>
class vec3
{
    T m[3];
public:
    vec3();
    vec3(T m0, T m1, T m2);

    T& x();
    T& y();
    T& z();

    vec3& operator= (const vec3 & v);

    T& operator[](int i);

    vec3& operator+= (const vec3 & v);
    vec3& operator-= (const vec3 & v);
    vec3& operator*= (T f);
    vec3& operator/= (T f);

    vec3 operator+ (vec3<T> v);
    vec3 operator- (vec3<T> v);
    vec3 operator* (T f);
    vec3 operator/ (T f);

    T squaredMagnitude();

    template<class U, class V>
    friend vec3<V> operator* (U f, const vec3<V> & v);
};

template<class T, class U>
vec3<U> operator* (T f, const vec3<U> & v) { return vec3<U>(v.m[0]*f, v.m[1]*f); }


#endif // VEC3_H
