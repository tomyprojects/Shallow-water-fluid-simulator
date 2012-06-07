#include "vec2.h"
#include <cassert>

template<class T>
vec2<T>::vec2()
{
    m[0] = 0;
    m[1] = 0;
}

template<class T>
vec2<T>::vec2(T m0, T m1)
{
    m[0] = m0; m[1] = m1;
}

template<class T>
T& vec2<T>::x()
{
    return m[0];
}

template<class T>
T& vec2<T>::y()
{
    return m[1];
}

template<class T>
vec2<T>& vec2<T>::operator= (const vec2 & v)
{   
    m[0] = v.m[0];
    m[1] = v.m[1];
    return *this;
}

template<class T>
T& vec2<T>::operator[](int i)
{
    assert(m[i] == m[i]);
    return m[i];
}

template<class T>
vec2<T>& vec2<T>::operator+= (const vec2 & v)
{
    m[0] += v.m[0];
    m[1] += v.m[1];
    return *this;
}

template<class T>
vec2<T>& vec2<T>::operator-= (const vec2 & v)
{
    m[0] -= v.m[0];
    m[1] -= v.m[1];
    return *this;
}

template<class T>
vec2<T>& vec2<T>::operator*= (T f)
{
    m[0] *= f;
    m[1] *= f;
    return *this;
}

template<class T>
vec2<T>& vec2<T>::operator/= (T f)
{
    m[0] /= f;
    m[1] /= f;
    return *this;
}

template<class T>
vec2<T> vec2<T>::operator+ (vec2<T> v)
{
    v[0] += m[0];
    v[1] += m[1];
    return v;
}

template<class T>
vec2<T> vec2<T>::operator- (vec2<T> v)
{
    v[0] = m[0] - v[0];
    v[1] = m[1] - v[1];
    return v;
}

template<class T>
vec2<T> vec2<T>::operator* (T f)
{
    return vec2(m[0]*f, m[1]*f);
}

template<class T>
vec2<T> vec2<T>::operator/ (T f)
{
    return vec2(m[0]/f, m[1]/f);
}

template<class T>
T vec2<T>::squaredMagnitude()
{
    return m[0] * m[0] + m[1] * m[1];
}

template class vec2<float>;
template class vec2<double>;
template class vec2<int>;
template class vec2<long>;
template class vec2<unsigned int>;
