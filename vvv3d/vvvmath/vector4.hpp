/*
 * File:   vector4.h
 * Author: vvv
 *
 * Created on May 9, 2014, 3:01 PM
 */

#ifndef VECTOR4_H
#define VECTOR4_H
#include "mathconstants.hpp"
#include "matrices_types.hpp"
#include "matrix44.hpp"
#include "vector3.hpp"
#include <cmath>
#include <ostream>

namespace vvv {

template <typename T>
class vector4 {
public:
    union {
        T vector[4];

        struct {
            T x;
            T y;
            T z;
            T w;
        };
        struct {
            T r;
            T g;
            T b;
            T a;
        };
    };

    vector4() : x(0), y(0), z(0), w(0) {}
    explicit vector4(T x) : x(x), y(x), z(x), w(x) {}

    vector4(T x, T y, T z, T w = 1) : x(x), y(y), z(z), w(w) {}

    vector4(const vector3<T>& v3, T w = 1) : x(v3.x), y(v3.y), z(v3.z), w(w) {}

    inline vector4& set(T x, T y, T z, T w = 1);

    template <typename U>
    vector4(const vector4<U>& o) : x(o.x), y(o.y), z(o.z), w(o.w)
    {
    }

    template <typename U>
    inline vector4<T>& operator=(const vector4<U>& b);
    inline vector4& operator+=(const vector4& b);
    inline vector4& operator-=(const vector4& b);

    inline vector4 operator*(const T& b) const;
    inline vector4 operator/(const T& b) const;
    inline vector4& operator*=(T s);
    inline vector4& operator/=(T s);

    template <typename U>
    inline vector4<T>& operator*=(const vector4<U>& b);
    template <typename U>
    inline vector4<T>& operator/=(const vector4<U>& b);

    template <typename U>
    inline vector4<T> operator*(const vector4<U>& b) const;
    template <typename U>
    inline vector4<T> operator/(const vector4<U>& b) const;

    inline vector4 operator+(const vector4& b) const;
    inline vector4 operator-(const vector4& b) const;

    inline vector4 operator+() const;
    inline vector4 operator-() const;
    inline bool operator==(const vector4& b) const;
    inline bool operator!=(const vector4& b) const;

    inline T length() const;
    inline T length2() const;
    inline T dot(const vector4& b) const;

    inline vector4& rotateX(T angle);
    inline vector4& rotateY(T angle);
    inline vector4& rotateZ(T angle);

    inline vector4 rotatedX(T angle) const;
    inline vector4 rotatedY(T angle) const;
    inline vector4 rotatedZ(T angle) const;

    inline vector4& normalize();

    inline static T length(T x, T y, T z, T w);

    vector4& apply(const matrix44<T>& m)
    {
        *this = m.transform(*this);
        return *this;
    }
};

template <typename T>
inline vector4<T>& vector4<T>::set(T x, T y, T z, T w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
    return *this;
}

template <typename T>
template <typename U>
inline vector4<T>& vector4<T>::operator=(const vector4<U>& b)
{
    x = b.x;
    y = b.y;
    z = b.z;
    w = b.w;
    return *this;
}

template <typename T>
template <typename U>
inline vector4<T>& vector4<T>::operator*=(const vector4<U>& b)
{
    x *= b.x;
    y *= b.y;
    z *= b.z;
    w *= b.w;
    return *this;
}

template <typename T>
template <typename U>
inline vector4<T>& vector4<T>::operator/=(const vector4<U>& b)
{
    x /= b.x;
    y /= b.y;
    z /= b.z;
    w /= b.w;
    return *this;
}

template <typename T>
template <typename U>
inline vector4<T> vector4<T>::operator*(const vector4<U>& b) const
{
    return vector4(x * b.x, y * b.y, z * b.z, w * b.w);
}

template <typename T>
template <typename U>
inline vector4<T> vector4<T>::operator/(const vector4<U>& b) const
{
    return vector4(x / b.x, y / b.y, z / b.z, w / b.w);
}

template <typename T>
inline vector4<T>& vector4<T>::operator+=(const vector4& b)
{
    x += b.x;
    y += b.y;
    z += b.z;
    w += b.w;
    return *this;
}

template <typename T>
inline vector4<T>& vector4<T>::operator-=(const vector4& b)
{
    x -= b.x;
    y -= b.y;
    z -= b.z;
    w -= b.w;
    return *this;
}

template <typename T>
inline vector4<T>& vector4<T>::operator*=(T s)
{
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
}

template <typename T>
inline vector4<T>& vector4<T>::operator/=(T s)
{
    x /= s;
    y /= s;
    z /= s;
    w /= s;
    return *this;
}

template <typename T>
inline vector4<T> vector4<T>::operator*(const T& b) const
{
    return vector4<T>(x * b, y * b, z * b, w * b);
}

template <typename T>
inline vector4<T> vector4<T>::operator/(const T& b) const
{
    return vector4<T>(x / b, y / b, z / b, w / b);
}

template <typename T>
inline vector4<T> vector4<T>::operator+(const vector4& b) const
{
    return vector4(x + b.x, y + b.y, z + b.z, w + b.w);
}

template <typename T>
inline vector4<T> vector4<T>::operator-(const vector4& b) const
{
    return vector4(x - b.x, y - b.y, z - b.z, w - b.w);
}

template <typename T>
inline vector4<T> vector4<T>::operator+() const
{
    return *this;
}

template <typename T>
inline vector4<T> vector4<T>::operator-() const
{
    return vector4<T>(-x, -y, -z, -w);
}

template <typename T>
inline bool vector4<T>::operator==(const vector4<T>& b) const
{
    return x == b.x && y == b.y && z == b.z && w == b.w;
}

template <typename T>
inline bool vector4<T>::operator!=(const vector4<T>& b) const
{
    return !operator==(b);
}

template <typename T>
inline T vector4<T>::length2() const
{
    return x * x + y * y + z * z + w * w;
}

template <typename T>
inline T vector4<T>::length() const
{
    return sqrt(length2());
}

template <typename T>
T vector4<T>::length(T x, T y, T z, T w)
{
    return sqrt(x * x + y * y + z * z + w * w);
}

template <typename T>
inline T vector4<T>::dot(const vector4& b) const
{
    return x * b.x + y * b.y + z * b.z;
}

template <typename T>
inline vector4<T>& vector4<T>::normalize()
{
    const T len = length();
    if (len > VVV_EPSILON) {
        (*this) /= len;
    }
    else {
        set(1, 0, 0, 0);
    }
    return *this;
}

template <typename T>
inline vector4<T>& vector4<T>::rotateX(T angle)
{
    matrix44<T> m;
    m.loadRotationX(angle);

    return apply(m);
}

template <typename T>
inline vector4<T>& vector4<T>::rotateY(T angle)
{
    matrix44<T> m;
    m.loadRotationY(angle);

    return apply(m);
}

template <typename T>
inline vector4<T>& vector4<T>::rotateZ(T angle)
{
    matrix44<T> m;
    m.loadRotationZ(angle);

    return apply(m);
}

template <typename T>
inline vector4<T> vector4<T>::rotatedX(T angle) const
{
    vector4<T> ret = *this;
    ret.rotateX(angle);
    return ret;
}

template <typename T>
inline vector4<T> vector4<T>::rotatedY(T angle) const
{
    vector4<T> ret = *this;
    ret.rotateY(angle);
    return ret;
}

template <typename T>
inline vector4<T> vector4<T>::rotatedZ(T angle) const
{
    vector4<T> ret = *this;
    ret.rotateZ(angle);
    return ret;
}
} // namespace vvv

template <typename T>
inline std::ostream& operator<<(std::ostream& stream, const vvv::vector4<T>& v)
{
    stream << "{ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " }";
    return stream;
}

#endif /* VECTOR4_H */
