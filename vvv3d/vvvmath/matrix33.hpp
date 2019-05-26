/*
 * File:   matrix33.h
 * Author: vvv
 *
 * Created on May 7, 2014, 6:03 PM
 */

#ifndef MATRIX33_H
#define MATRIX33_H
#include "matrices_types.hpp"
#include <algorithm>
#include <ostream>

namespace vvv {

template <typename T>
class matrix33 {
public:
    T matrix[3][3];

    matrix33() { loadIdentity(); }

    matrix33<T>& loadIdentity()
    {
        matrix[0][0] = 1;
        matrix[0][1] = 0;
        matrix[0][2] = 0;
        matrix[1][0] = 0;
        matrix[1][1] = 1;
        matrix[1][2] = 0;
        matrix[2][0] = 0;
        matrix[2][1] = 0;
        matrix[2][2] = 1;
        return *this;
    }

    matrix33<T>& loadScale(T x, T y, T z)
    {
        matrix[0][0] = x;
        matrix[0][1] = 0;
        matrix[0][2] = 0;
        matrix[1][0] = 0;
        matrix[1][1] = y;
        matrix[1][2] = 0;
        matrix[2][0] = 0;
        matrix[2][1] = 0;
        matrix[2][2] = z;
        return *this;
    }

    matrix33<T>& loadRotation(T angle, T ax, T ay, T az);

    matrix33<T>& loadRotationX(T angle);
    matrix33<T>& loadRotationY(T angle);
    matrix33<T>& loadRotationZ(T angle);

    T determinant() const
    {
        return matrix[0][0] * matrix[1][1] * matrix[2][2] -
               matrix[0][0] * matrix[1][2] * matrix[2][1] -
               matrix[0][1] * matrix[1][0] * matrix[2][2] +
               matrix[0][1] * matrix[1][2] * matrix[2][0] +
               matrix[0][2] * matrix[1][0] * matrix[2][1] -
               matrix[0][2] * matrix[1][1] * matrix[2][0];
    }

    matrix33& invert();

    matrix33 inverted() const
    {
        matrix33 ret(*this);
        ret.invert();
        return ret;
    }

    matrix33& transpose()
    {
        std::swap(matrix[0][1], matrix[1][0]);
        std::swap(matrix[0][2], matrix[2][0]);
        std::swap(matrix[1][2], matrix[2][1]);
        return *this;
    }

    matrix33 transposed() const
    {
        matrix33 ret(*this);
        ret.transpose();
        return ret;
    }

    static matrix33<T> createMatrixFromRows(const vector3<T>& v0,
                                            const vector3<T>& v1,
                                            const vector3<T>& v2);
    static matrix33<T> createMatrixFromColumns(const vector3<T>& v0,
                                               const vector3<T>& v1,
                                               const vector3<T>& v2);

    static void mul(const matrix33<T>& lhs, const matrix33<T>& rhs,
                    matrix33<T>& out);

    matrix33<T> operator*(const matrix33& rhs) const
    {
        matrix33<T> ret;
        mul(*this, rhs, ret);
        return ret;
    }

    matrix33& operator*=(const matrix33& rhs)
    {
        mul(*this, rhs, *this);
        return *this;
    }

    vector3<T> transform(const vector3<T>& in) const;

    vector3<T> operator*(const vector3<T>& in) const { return transform(in); }

    void transform(const vector3<T>& in, vector3<T>& out) const;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& stream, const vvv::matrix33<T>& m)
{
    stream << "{ { " << m.matrix[0][0] << ", " << m.matrix[0][1] << ", "
           << m.matrix[0][2] << "}, " << std::endl;
    stream << "  { " << m.matrix[1][0] << ", " << m.matrix[1][1] << ", "
           << m.matrix[1][2] << "}, " << std::endl;
    stream << "  { " << m.matrix[2][0] << ", " << m.matrix[2][1] << ", "
           << m.matrix[2][2] << "}, " << std::endl;
    return stream;
}

typedef matrix33<double> matrix33d;
typedef matrix33<float> matrix33f;
} // namespace vvv

#endif /* MATRIX33_H */
