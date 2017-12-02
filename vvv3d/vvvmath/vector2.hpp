/*
 * File:   vector2.h
 * Author: vvv
 *
 * Created on May 7, 2014, 1:43 PM
 */

#ifndef VECTOR2_H
#define VECTOR2_H
#include <cmath>
#include <ostream>
#define VVV_EPSILON 0.5e-15

namespace vvv {
template <typename T>
class vector2 {
public:
    union {
        T vector[2];
        struct {
            T x;
            T y;
        };
    };

    vector2() {}
    explicit vector2(T x) : x(x), y(x) {}
    vector2(T x, T y) : x(x), y(y) {}

    inline vector2<T>& set(T x, T y);

    inline vector2<T>& operator+=(const vector2<T>& b);
    inline vector2<T>& operator-=(const vector2<T>& b);

    inline vector2<T>& operator*=(T s);
    inline vector2<T>& operator/=(T s);

    inline vector2<T>& operator*=(const vector2<T>& b);
    inline vector2<T>& operator/=(const vector2<T>& b);

    inline vector2<T> operator*(const vector2& b) const;
    inline vector2<T> operator/(const vector2& b) const;

    inline vector2<T> operator+(const vector2<T>& b) const;
    inline vector2<T> operator-(const vector2<T>& b) const;

    inline vector2<T> operator+() const;
    inline vector2<T> operator-() const;
    inline bool operator==(const vector2& b) const;
    inline bool operator!=(const vector2& b) const;

    inline T length() const;
    inline T dot(const vector2<T>& b) const;

    inline vector2<T>& rotate(T angle);
    inline vector2<T> rotated(T angle) const;

    inline vector2<T>& normalize();
};

template <typename T>
inline vector2<T>& vector2<T>::set(T x, T y)
{
    this->x = x;
    this->y = y;
    return *this;
}

template <typename T>
inline vector2<T>& vector2<T>::operator+=(const vector2<T>& b)
{
    x += b.x;
    y += b.y;
    return *this;
}

template <typename T>
inline vector2<T>& vector2<T>::operator-=(const vector2<T>& b)
{
    x -= b.x;
    y -= b.y;
    return *this;
}

template <typename T>
inline vector2<T>& vector2<T>::operator*=(const vector2<T>& b)
{
    x *= b.x;
    y *= b.y;
    return *this;
}

template <typename T>
inline vector2<T>& vector2<T>::operator/=(const vector2<T>& b)
{
    x /= b.x;
    y /= b.y;
    return *this;
}

template <typename T>
inline vector2<T>& vector2<T>::operator*=(T s)
{
    x *= s;
    y *= s;
    return *this;
}

template <typename T>
inline vector2<T>& vector2<T>::operator/=(T s)
{
    x /= s;
    y /= s;
    return *this;
}

template <typename T>
inline vector2<T> vector2<T>::operator+(const vector2<T>& b) const
{
    return vector2<T>(x + b.x, y + b.y);
}

template <typename T>
inline vector2<T> vector2<T>::operator-(const vector2<T>& b) const
{
    return vector2<T>(x - b.x, y - b.y);
}

template <typename T>
inline vector2<T> vector2<T>::operator*(const vector2<T>& b) const
{
    return vector2<T>(x * b.x, y * b.y);
}

template <typename T>
inline vector2<T> vector2<T>::operator/(const vector2<T>& b) const
{
    return vector2<T>(x / b.x, y / b.y);
}

template <typename T>
inline vector2<T> vector2<T>::operator+() const
{
    return *this;
}

template <typename T>
inline vector2<T> vector2<T>::operator-() const
{
    return vector2<T>(-x, -y);
}
template <typename T>
inline bool vector2<T>::operator==(const vector2& b) const
{
    return x == b.x && y == b.y;
}
template <typename T>
inline bool vector2<T>::operator!=(const vector2& b) const
{
    return x != b.x || y != b.y;
}

template <typename T>
inline T vector2<T>::length() const
{
    return sqrt(x * x + y * y);
}

template <typename T>
inline T vector2<T>::dot(const vector2<T>& b) const
{
    return x * b.x + y * b.y;
}

template <typename T>
inline vector2<T>& vector2<T>::rotate(T angle)
{
    const T c = cos(angle);
    const T s = sin(angle);

    const T x = c * this->x - s * this->y;
    const T y = s * this->x + c * this->y;

    this->x = x;
    this->y = y;

    return *this;
}

template <typename T>
inline vector2<T> vector2<T>::rotated(T angle) const
{
    const T c = cos(angle);
    const T s = sin(angle);

    const T x = c * this->x - s * this->y;
    const T y = s * this->x + c * this->y;

    return vector2<T>(x, y);
}

template <typename T>
inline vector2<T>& vector2<T>::normalize()
{
    const T len = length();
    if (len > VVV_EPSILON) {
        (*this) /= len;
    }
    else {
        set(1, 0);
    }
    return *this;
}
}

template <typename T>
inline std::ostream& operator<<(std::ostream& stream, const vvv::vector2<T>& v)
{
    stream << "{ " << v.x << ", " << v.y << " }";
    return stream;
}

#endif /* VECTOR2_H */
