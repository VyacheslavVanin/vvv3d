/*
 * File:   vector3.h
 * Author: vvv
 *
 * Created on May 7, 2014, 2:36 PM
 */

#ifndef VECTOR3_H
#define VECTOR3_H
#include "functions.hpp"
#include "mathconstants.hpp"
#include "matrices_types.hpp"
#include <cmath>
#include <ostream>

namespace vvv {
template <typename T>
class vector3 {
public:
    union {
        T vector[3];

        struct {
            T x;
            T y;
            T z;
        };

        struct {
            T r;
            T g;
            T b;
        };
    };

    vector3() {}

    explicit vector3(T x) : x(x), y(x), z(x) {}

    vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    explicit vector3(const T* v) : x(v[0]), y(v[1]), z(v[0]) {}

    inline vector3<T>& set(T x, T y, T z);

    template <typename U>
    vector3(const vector3<U>& o) : x(o.x), y(o.y), z(o.z)
    {
    }

    template <typename U>
    inline vector3<T>& operator=(const vector3<U>& b);
    inline vector3<T>& operator+=(const vector3& b);
    inline vector3<T>& operator-=(const vector3& b);

    inline vector3<T>& operator*=(T s);
    inline vector3<T>& operator/=(T s);

    inline vector3<T>& operator*=(const vector3& b);
    inline vector3<T>& operator/=(const vector3& b);

    inline vector3<T> operator*(const vector3& b) const;
    inline vector3<T> operator/(const vector3& b) const;

    inline vector3<T> operator+(const vector3& b) const;
    inline vector3<T> operator-(const vector3& b) const;

    inline vector3<T> operator+() const;
    inline vector3<T> operator-() const;
    inline bool operator==(const vector3& b) const;
    inline bool operator!=(const vector3& b) const;

    inline T length() const;
    inline T length2() const;
    inline T dot(const vector3& b) const;
    inline vector3<T> cross(const vector3& b) const;
    static inline vector3<T> cross(const vector3& a, const vector3& b);

    vector3& rotateX(T angle);
    vector3& rotateY(T angle);
    vector3& rotateZ(T angle);

    vector3 rotatedX(T angle) const;
    vector3 rotatedY(T angle) const;
    vector3 rotatedZ(T angle) const;

    inline vector3<T>& normalize();
    inline vector3<T> normalized() const;

    inline static T length(T x, T y, T z);
    inline static T angle(const vector3& a, const vector3& b);

    vector3<T>& apply(const matrix33<T>& m);

    inline T lengthXY() const;
    inline T lengthXZ() const;
    inline T lengthYZ() const;
};

template <typename T>
inline vector3<T>& vector3<T>::set(T x, T y, T z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    return *this;
}

template <typename T>
template <typename U>
inline vector3<T>& vector3<T>::operator=(const vector3<U>& b)
{
    x = b.x;
    y = b.y;
    z = b.z;
    return *this;
}

template <typename T>
inline vector3<T>& vector3<T>::operator+=(const vector3& b)
{
    x += b.x;
    y += b.y;
    z += b.z;
    return *this;
}

template <typename T>
inline vector3<T>& vector3<T>::operator-=(const vector3& b)
{
    x -= b.x;
    y -= b.y;
    z -= b.z;
    return *this;
}

template <typename T>
inline vector3<T>& vector3<T>::operator*=(const vector3& b)
{
    x *= b.x;
    y *= b.y;
    z *= b.z;
    return *this;
}

template <typename T>
inline vector3<T>& vector3<T>::operator/=(const vector3& b)
{
    x /= b.x;
    y /= b.y;
    z /= b.z;
    return *this;
}

template <typename T>
inline vector3<T>& vector3<T>::operator*=(T s)
{
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

template <typename T>
inline vector3<T>& vector3<T>::operator/=(T s)
{
    x /= s;
    y /= s;
    z /= s;
    return *this;
}

template <typename T>
inline vector3<T> vector3<T>::operator+(const vector3& b) const
{
    return vector3<T>(x + b.x, y + b.y, z + b.z);
}

template <typename T>
inline vector3<T> vector3<T>::operator-(const vector3& b) const
{
    return vector3<T>(x - b.x, y - b.y, z - b.z);
}

template <typename T>
inline vector3<T> vector3<T>::operator*(const vector3& b) const
{
    return vector3<T>(x * b.x, y * b.y, z * b.z);
}

template <typename T>
inline vector3<T> vector3<T>::operator/(const vector3& b) const
{
    return vector3<T>(x / b.x, y / b.y, z / b.z);
}

template <typename T>
inline vector3<T> vector3<T>::operator+() const
{
    return *this;
}

template <typename T>
inline vector3<T> vector3<T>::operator-() const
{
    return vector3<T>(-x, -y, -z);
}

template <typename T>
inline bool vector3<T>::operator==(const vector3<T>& b) const
{
    return x == b.x && y == b.y && z == b.z;
}

template <typename T>
inline bool vector3<T>::operator!=(const vector3<T>& b) const
{
    return !operator==(b);
}

template <typename T>
inline T vector3<T>::length2() const
{
    return x * x + y * y + z * z;
}

template <typename T>
inline T vector3<T>::length() const
{
    return sqrt(x * x + y * y + z * z);
}

template <typename T>
T vector3<T>::length(T x, T y, T z)
{
    return sqrt(x * x + y * y + z * z);
}

template <typename T>
inline T vector3<T>::dot(const vector3& b) const
{
    return x * b.x + y * b.y + z * b.z;
}

template <typename T>
inline vector3<T> vector3<T>::cross(const vector3& b) const
{
    const T ox = y * b.z - z * b.y;
    const T oy = z * b.x - x * b.z;
    const T oz = x * b.y - y * b.x;
    return vector3<T>(ox, oy, oz);
}

template <typename T>
inline vector3<T> vector3<T>::cross(const vector3& a, const vector3& b)
{
    return a.cross(b);
}

template <typename T>
inline vector3<T>& vector3<T>::normalize()
{
    const T len = length();
    if (len > VVV_EPSILON) {
        (*this) /= len;
    }
    else {
        set(1, 0, 0);
    }
    return *this;
}

template <typename T>
vector3<T> vector3<T>::normalized() const
{
    return vector3<T>(*this).normalize();
}

template <typename T>
inline T vector3<T>::lengthXY() const
{
    return sqrt(x * x + y * y);
}

template <typename T>
inline T vector3<T>::lengthXZ() const
{
    return sqrt(x * x + z * z);
}

template <typename T>
inline T vector3<T>::lengthYZ() const
{
    return sqrt(y * y + z * z);
}

template <typename T>
inline T vector3<T>::angle(const vvv::vector3<T>& a, const vvv::vector3<T>& b)
{
    return acos(clamp_fast(-1.0f, 1.0f, a.dot(b) / (a.length() * b.length())));
}
} // namespace vvv

template <typename T>
inline vvv::vector3<T> operator*(const vvv::vector3<T>& a, const T& s)
{
    return vvv::vector3<T>(a.x * s, a.y * s, a.z * s);
}

template <typename T>
inline vvv::vector3<T> operator*(const T& s, const vvv::vector3<T>& a)
{
    return vvv::vector3<T>(a.x * s, a.y * s, a.z * s);
}

template <typename T>
inline vvv::vector3<T> operator/(const vvv::vector3<T>& a, const T& s)
{
    return vvv::vector3<T>(a.x / s, a.y / s, a.z / s);
}

template <typename T>
inline vvv::vector3<T> operator/(const T& s, const vvv::vector3<T>& a)
{
    return vvv::vector3<T>(s / a.x, s / a.y, s / a.z);
}

namespace vvv {
template <typename T>
inline std::ostream& operator<<(std::ostream& stream, const vvv::vector3<T>& v)
{
    stream << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
    return stream;
}
} // namespace vvv

#endif /* VECTOR3_H */
