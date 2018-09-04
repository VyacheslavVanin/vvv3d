/*
 * File:   matrix44.h
 * Author: vvv
 *
 * Created on May 9, 2014, 1:57 PM
 */

#ifndef MATRIX44_H
#define MATRIX44_H
#include "mathconstants.hpp"
#include "matrices_types.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <string.h>

namespace vvv {

template <typename T>
class matrix44 {
public:
    T matrix[4][4];

    matrix44() { loadIdentity(); }

    matrix44<T>& loadIdentity()
    {
        memset(matrix, 0, sizeof(matrix));
        matrix[0][0] = 1;
        matrix[1][1] = 1;
        matrix[2][2] = 1;
        matrix[3][3] = 1;
        return *this;
    }

    matrix44<T>& loadRotation(T angle, T ax, T ay, T az);

    matrix44<T>& loadRotationX(T angle);
    matrix44<T>& loadRotationY(T angle);
    matrix44<T>& loadRotationZ(T angle);
    matrix44<T>& loadScale(const T& x, const T& y, const T& z);
    matrix44<T>& loadScale(const vector3<T>& s);
    matrix44<T>& loadTranslation(const vector3<T>& to);
    matrix44<T>& loadTranslation(const T& x, const T& y, const T& z);
    matrix44<T>& loadFromColumns(const vector4<T>& v0, const vector4<T>& v1,
                                 const vector4<T>& v2, const vector4<T>& v3);
    matrix44<T>& loadLookAt(const vector3<T>& eye, const vector3<T>& target,
                            const vector3<T>& up);

    matrix44<T>& loadOrtho(T left, T right, T bottom, T top, T zNear, T zFar)
    {
        matrix[0][0] = 2 / (right - left);
        matrix[0][1] = 0;
        matrix[0][2] = 0;
        matrix[0][3] = 0;
        matrix[1][0] = 0;
        matrix[1][1] = 2 / (top - bottom);
        matrix[1][2] = 0;
        matrix[1][3] = 0;
        matrix[2][0] = 0;
        matrix[2][1] = 0;
        matrix[2][2] = -2 / (zFar - zNear);
        matrix[2][3] = 0;
        matrix[3][0] = -(right + left) / (right - left);
        matrix[3][1] = -(top + bottom) / (top - bottom);
        matrix[3][2] = -(zFar + zNear) / (zFar - zNear);
        matrix[3][3] = 1;
        return *this;
    }

    matrix44<T>& loadPerspective(T fovy, T aspect, T zNear, T zFar)
    {
        const T f = 1 / tan(fovy * M_PI / 360);
        const T A = (zFar + zNear) / (zNear - zFar);
        const T B = (2 * zFar * zNear) / (zNear - zFar);
        memset(matrix, 0, sizeof(matrix));
        matrix[0][0] = f / aspect;
        matrix[1][1] = f;
        matrix[2][2] = A;
        matrix[2][3] = -1;
        matrix[3][2] = B;
        return *this;
    }

    T determinant() const;

    matrix44<T>& invert();

    matrix44<T> inverted() const { return matrix44<T>(*this).invert(); }

    matrix44<T>& transpose()
    {
        std::swap(matrix[0][1], matrix[1][0]);
        std::swap(matrix[0][2], matrix[2][0]);
        std::swap(matrix[0][3], matrix[3][0]);
        std::swap(matrix[1][2], matrix[2][1]);
        std::swap(matrix[1][3], matrix[3][1]);
        std::swap(matrix[2][3], matrix[3][2]);

        return *this;
    }

    matrix44<T> transposed() const { return matrix44<T>(*this).transpose(); }

    static void mul(const matrix44<T>& lhs, const matrix44<T>& rhs,
                    matrix44<T>& out);

    matrix44<T> operator*(const matrix44<T>& rhs) const
    {
        matrix44<T> ret;
        mul(*this, rhs, ret);
        return ret;
    }

    matrix44<T>& operator*=(const matrix44& rhs)
    {
        mul(*this, rhs, *this);
        return *this;
    }

    vector4<T> operator*(const vector4<T>& in) const { return transform(in); }

    vector3<T> operator*(const vector3<T>& in) const { return transform(in); }

    vector4<T> transform(const vector4<T>& in) const;
    vector3<T> transform(const vector3<T>& in) const;
    void transform(const vector4<T>& in, vector4<T>& out) const;
    void transform(const vector3<T>& in, vector3<T>& out) const;

    matrix33<T> getRotationPart() const;

    static matrix44<T> createPerspectiveMatrix(T fovy, T aspect, T zNear,
                                               T zFar)
    {
        return matrix44<T>().loadPerspective(fovy, aspect, zNear, zFar);
    }

    static matrix44<T> createOrthoMatrix(T left, T right, T bottom, T top,
                                         T zNear, T zFar)
    {
        return matrix44<T>().loadOrtho(left, right, bottom, top, zNear, zFar);
    }

    static matrix44<T> createLookAtMatrix(const vector3<T>& eye,
                                          const vector3<T>& target,
                                          const vector3<T>& up)
    {
        return matrix44<T>().loadLookAt(eye, target, up);
    }

    static matrix44<T> createIdentityMatrix()
    {
        return matrix44<T>().loadIdentity();
    }

    static matrix44<T> createRotationMatrix(const T& angle, const T& ax,
                                            const T& ay, const T& az)
    {
        return matrix44<T>().loadRotation(angle, ax, ay, az);
    }

    static matrix44<T> createTranslateMatrix(const vector3<T>& to)
    {
        return matrix44<T>().loadTranslation(to);
    }

    static matrix44<T> createTranslateMatrix(const T& x, const T& y, const T& z)
    {
        return matrix44<T>().loadTranslation(x, y, z);
    }

    static matrix44<T> createScaleMatrix(const vector3<T>& s)
    {
        return matrix44<T>().loadScale(s);
    }

    static matrix44<T> createScaleMatrix(const T& x, const T& y, const T& z)
    {
        return matrix44<T>().loadScale(x, y, z);
    }
};

typedef matrix44<double> matrix44d;
typedef matrix44<float> matrix44f;
} // namespace vvv

template <typename T>
inline std::ostream& operator<<(std::ostream& stream, const vvv::matrix44<T>& m)
{
    stream << "{ { " << m.matrix[0][0] << ", " << m.matrix[0][1] << ", "
           << m.matrix[0][2] << ", " << m.matrix[0][3] << "}, " << std::endl;
    stream << "  { " << m.matrix[1][0] << ", " << m.matrix[1][1] << ", "
           << m.matrix[1][2] << ", " << m.matrix[1][3] << "}, " << std::endl;
    stream << "  { " << m.matrix[2][0] << ", " << m.matrix[2][1] << ", "
           << m.matrix[2][2] << ", " << m.matrix[2][3] << "}, " << std::endl;
    stream << "  { " << m.matrix[3][0] << ", " << m.matrix[3][1] << ", "
           << m.matrix[3][2] << ", " << m.matrix[3][3] << "} }" << std::endl;
    return stream;
}

#endif /* MATRIX44_H */
