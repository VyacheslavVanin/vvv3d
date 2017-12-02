#ifndef QUATERNION_H
#define QUATERNION_H
#include "vector3.hpp"
#include "vector4.hpp"

namespace vvv {
template <typename T>
class quaternion {
public:
    quaternion() : q() {}
    quaternion(T x, T y, T z, T w) : q(x, y, z, w) {}
    quaternion(const vector4<T>& v) : q(v) {}
    quaternion(T angle, const vector3<T>& axis)
        : q(axis.normalized() * std::sin(angle / 2), std::cos(angle / 2))
    {
    }

    inline quaternion<T>& set(T x, T y, T z, T w)
    {
        q.set(x, y, z, w);
        return *this;
    }
    inline quaternion<T>& setEuler(T pitch, T yaw, T roll);
    inline quaternion<T>& setAngleAxis(T angle, const vector3<T>& axis);

    inline quaternion<T>& operator+=(const quaternion& b);
    inline quaternion<T>& operator-=(const quaternion& b);
    inline quaternion<T>& operator*=(const quaternion& b);
    inline quaternion<T>& operator*=(T s);
    inline quaternion<T>& operator/=(T s);

    inline quaternion<T> operator*(const quaternion& b) const;
    inline quaternion<T> operator+(const quaternion& b) const;
    inline quaternion<T> operator-(const quaternion& b) const;

    inline T length2() const;
    inline T length() const;
    inline T dot(const quaternion<T>& b) const;

    inline quaternion<T>& normalize();
    inline quaternion<T>& conjugate();
    inline quaternion<T>& inverse();

    inline vector3<T> getAxis() const;
    inline T getAngle() const;

    inline void toMatrix33(vvv::matrix33<T>& out) const;
    inline void toMatrix44(vvv::matrix44<T>& out) const;

private:
    vector4<T> q;
};

template <typename T>
quaternion<T>& quaternion<T>::setEuler(T pitch, T yaw, T roll)
{
    const auto tmp4 = sin(pitch * 0.5);
    const auto tmp1 = cos(pitch * 0.5);
    const auto tmp5 = sin(yaw * 0.5);
    const auto tmp2 = cos(yaw * 0.5);
    const auto tmp6 = sin(roll * 0.5);
    const auto tmp3 = cos(roll * 0.5);
    return set(tmp4 * tmp2 * tmp3 + tmp1 * tmp5 * tmp6,
               tmp1 * tmp5 * tmp3 + tmp4 * tmp2 * tmp6,
               tmp1 * tmp2 * tmp6 + tmp4 * tmp5 * tmp3,
               tmp1 * tmp2 * tmp3 + tmp4 * tmp5 * tmp6);
}

template <typename T>
quaternion<T>& quaternion<T>::setAngleAxis(T angle, const vector3<T>& axis)
{
    q = vector4<T>(axis.normalized() * std::sin(angle / 2),
                   std::cos(angle / 2));
    return *this;
}

template <typename T>
quaternion<T>& quaternion<T>::operator+=(const quaternion& b)
{
    q += b.q;
    return *this;
}

template <typename T>
quaternion<T>& quaternion<T>::operator-=(const quaternion& b)
{
    q -= b.q;
    return *this;
}

template <typename T>
quaternion<T>& quaternion<T>::operator*=(const quaternion& p)
{
    q.set(q.w * p.q.x + q.x * p.q.w + q.y * p.q.z - q.z * p.q.y,
          q.w * p.q.y + q.y * p.q.w + q.z * p.q.x - q.x * p.q.z,
          q.w * p.q.z + q.z * p.q.w + q.x * p.q.y - q.y * p.q.x,
          q.w * p.q.w - q.x * p.q.x - q.y * p.q.y - q.z * p.q.z);
    return *this;
}

template <typename T>
quaternion<T>& quaternion<T>::operator*=(T s)
{
    q *= s;
    return *this;
}

template <typename T>
quaternion<T>& quaternion<T>::operator/=(T s)
{
    q /= s;
    return *this;
}

template <typename T>
quaternion<T> quaternion<T>::operator*(const quaternion& p) const
{
    return quaternion<T>(q.w * p.x + q.x * p.w + q.y * p.z - q.z * p.y,
                         q.w * p.y + q.y * p.w + q.z * p.x - q.x * p.z,
                         q.w * p.z + q.z * p.w + q.x * p.y - q.y * p.x,
                         q.w * p.w - q.x * p.x - q.y * p.y - q.z * p.z);
}

template <typename T>
quaternion<T> quaternion<T>::operator+(const quaternion& b) const
{
    return quaternion<T>(q + b.q);
}

template <typename T>
quaternion<T> quaternion<T>::operator-(const quaternion& b) const
{
    return quaternion<T>(q - b.q);
}

template <typename T>
T quaternion<T>::length2() const
{
    return q.length2();
}

template <typename T>
T quaternion<T>::length() const
{
    return q.length();
}

template <typename T>
T quaternion<T>::dot(const quaternion<T>& b) const
{
    return q.dot(b.q);
}

template <typename T>
quaternion<T>& quaternion<T>::normalize()
{
    q.normalize();
    return *this;
}

template <typename T>
quaternion<T>& quaternion<T>::conjugate()
{
    q.x = -q.x;
    q.y = -q.y;
    q.z = -q.z;
    return *this;
}

template <typename T>
quaternion<T>& quaternion<T>::inverse()
{
    return conjugate() *= 1 / q.length2();
}

template <typename T>
vector3<T> quaternion<T>::getAxis() const
{
    const auto t1 = 1 - q.w * q.w;
    return (t1 <= 0) ? 0 : q / std::sqrt(t1);
}

template <typename T>
T quaternion<T>::getAngle() const
{
    return 2 * std::acos(q.w);
}
}

#endif // QUATERNION_H
