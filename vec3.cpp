#include "vec3.h"

#include <cassert>

template<class T>
vec3<T>::vec3()
{
    m[0] = 0;
    m[1] = 0;
}

template<class T>
vec3<T>::vec3(T m0, T m1, T m2)
{
    m[0] = m0; m[1] = m1; m[2] = m2;
}

template<class T>
T& vec3<T>::x()
{
    return m[0];
}

template<class T>
T& vec3<T>::y()
{
    return m[1];
}

template<class T>
T& vec3<T>::z()
{
    return m[2];
}

template<class T>
vec3<T>& vec3<T>::operator= (const vec3 & v)
{
    m[0] = v.m[0];
    m[1] = v.m[1];
    m[2] = v.m[2];
    return *this;
}

template<class T>
T& vec3<T>::operator[](int i)
{
    assert(m[i] == m[i]);
    return m[i];
}

template<class T>
vec3<T>& vec3<T>::operator+= (const vec3 & v)
{
    m[0] += v.m[0];
    m[1] += v.m[1];
    m[2] += v.m[2];
    return *this;
}

template<class T>
vec3<T>& vec3<T>::operator-= (const vec3 & v)
{
    m[0] -= v.m[0];
    m[1] -= v.m[1];
    m[2] -= v.m[2];
    return *this;
}

template<class T>
vec3<T>& vec3<T>::operator*= (T f)
{
    m[0] *= f;
    m[1] *= f;
    m[2] *= f;
    return *this;
}

template<class T>
vec3<T>& vec3<T>::operator/= (T f)
{
    m[0] /= f;
    m[1] /= f;
    m[2] /= f;
    return *this;
}

template<class T>
vec3<T> vec3<T>::operator+ (vec3<T> v)
{
    v[0] += m[0];
    v[1] += m[1];
    v[2] += m[2];
    return v;
}

template<class T>
vec3<T> vec3<T>::operator- (vec3<T> v)
{
    v[0] = m[0] - v[0];
    v[1] = m[1] - v[1];
    v[2] = m[2] - v[2];
    return v;
}

template<class T>
vec3<T> vec3<T>::operator* (T f)
{
    return vec3(m[0]*f, m[1]*f, m[2]*f);
}

template<class T>
vec3<T> vec3<T>::operator/ (T f)
{
    return vec3(m[0]/f, m[1]/f, m[2]/f);
}

template<class T>
T vec3<T>::squaredMagnitude()
{
    return m[0] * m[0] + m[1] * m[1] + m[2] * m[2];
}

template class vec3<float>;
template class vec3<double>;
template class vec3<int>;
template class vec3<long>;
template class vec3<unsigned int>;
