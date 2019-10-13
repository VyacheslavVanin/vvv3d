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

    inline const vector4<T>& getRaw() const;

private:
    vector4<T> q;
};

template <typename T>
quaternion<T>& quaternion<T>::setEuler(T pitch, T yaw, T roll)
{
    // Abbreviations for the various angular functions
    const double cy = cos(yaw * 0.5);
    const double sy = sin(yaw * 0.5);
    const double cp = cos(pitch * 0.5);
    const double sp = sin(pitch * 0.5);
    const double cr = cos(roll * 0.5);
    const double sr = sin(roll * 0.5);

    return set(cy * cp * sr - sy * sp * cr, sy * cp * sr + cy * sp * cr,
               sy * cp * cr - cy * sp * sr, cy * cp * cr + sy * sp * sr);
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
    const auto& q0 = p.q;
    const auto& q1 = q;
    q.set(q0.w * q1.x + q0.x * q1.w + q0.y * q1.z - q0.z * q1.y,
          q0.w * q1.y + q0.y * q1.w + q0.z * q1.x - q0.x * q1.z,
          q0.w * q1.z + q0.z * q1.w + q0.x * q1.y - q0.y * q1.x,
          q0.w * q1.w - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z);
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
    quaternion<T> ret(*this);
    return ret *= p;
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

template <typename T>
inline const vector4<T>& quaternion<T>::getRaw() const
{
    return q;
}
} // namespace vvv

#endif // QUATERNION_H
