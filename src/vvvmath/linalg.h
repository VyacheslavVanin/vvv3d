#ifndef LINALG_H
#define LINALG_H
#include "matrices_types.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix33.h"
#include "matrix44.h"
#include "quaternion.h"

namespace vvv
{
    template<typename T>
    inline vector3<T>& vector3<T>::apply(const matrix33<T>& m)
    {
        *this = m.transform(*this);
        return *this;
    }

    template<typename T>
    inline vector3<T>& vector3<T>::rotateX(T angle)
    {
        matrix33<T> m;
        m.loadRotationX(angle);

        return apply(m);
    }

    template<typename T>
    inline vector3<T>& vector3<T>::rotateY(T angle)
    {
        matrix33<T> m;
        m.loadRotationY(angle);

        return apply(m);
    }

    template<typename T>
    inline vector3<T>& vector3<T>::rotateZ(T angle)
    {
        matrix33<T> m;
        m.loadRotationZ(angle);

        return apply(m);
    }

    template<typename T>
    inline vector3<T> vector3<T>::rotatedX(T angle) const
    {
        vector3<T> ret = *this;
        ret.rotateX(angle);
        return ret;
    }

    template<typename T>
    inline vector3<T> vector3<T>::rotatedY(T angle) const
    {
        vector3<T> ret = *this;
        ret.rotateY(angle);
        return ret;
    }

    template<typename T>
    inline vector3<T> vector3<T>::rotatedZ(T angle) const
    {
        vector3<T> ret = *this;
        ret.rotateZ(angle);
        return ret;
    }


    template<typename T>
    inline vector3<T> matrix33<T>::transform(const vector3<T>& in) const
    {
        const T x = matrix[0][0] * in.x + matrix[1][0] * in.y + matrix[2][0] * in.z;
        const T y = matrix[0][1] * in.x + matrix[1][1] * in.y + matrix[2][1] * in.z;
        const T z = matrix[0][2] * in.x + matrix[1][2] * in.y + matrix[2][2] * in.z;
        return vector3<T>(x, y, z);
    }

    template<typename T>
    inline void    matrix33<T>::transform( const vector3<T>& in, vector3<T>& out) const
    {
        const T x = matrix[0][0] * in.x + matrix[1][0] * in.y + matrix[2][0] * in.z;
        const T y = matrix[0][1] * in.x + matrix[1][1] * in.y + matrix[2][1] * in.z;
        const T z = matrix[0][2] * in.x + matrix[1][2] * in.y + matrix[2][2] * in.z;
        out.set(x,y,z);
    }


    template<typename T>
    inline matrix33<T>& matrix33<T>::loadRotation(T angle, T ax, T ay, T az)
    {
        const T c = cos(angle);
        const T s = sin(angle);
        const T al = vector3<T>::length(ax, ay, az);

        if (al != 0)
        {
            ax /= al;
            ay /= al;
            az /= al;
        }

        const T axcm = (1.0 - c) * ax;
        const T azcm = (1.0 - c) * az;

        matrix[0][0] = c + axcm*ax;
        matrix[0][1] = axcm * ay + s*az;
        matrix[0][2] = axcm * az - s*ay;

        matrix[1][0] = axcm * ay - s*az;
        matrix[1][1] = c + (1.0f - c) * ay*ay;
        matrix[1][2] = azcm * ay + s*ax;

        matrix[2][0] = axcm * az + s*ay;
        matrix[2][1] = azcm * ay - s*ax;
        matrix[2][2] = c + azcm*az;
        return *this;
    }

    template<typename T>
    inline matrix33<T>& matrix33<T>::loadRotationX(T angle)
    {
        const T c = cos(angle);
        const T s = sin(angle);

        matrix[0][0] = 1;
        matrix[0][1] = 0;
        matrix[0][2] = 0;

        matrix[1][0] = 0;
        matrix[1][1] = c;
        matrix[1][2] = s;

        matrix[2][0] = 0;
        matrix[2][1] = -s;
        matrix[2][2] = c;
        return *this;
    }

    template<typename T>
    inline matrix33<T>& matrix33<T>::loadRotationY(T angle)
    {
        const T c = cos(angle);
        const T s = sin(angle);

        matrix[0][0] = c;
        matrix[0][1] = 0;
        matrix[0][2] = -s;

        matrix[1][0] = 0;
        matrix[1][1] = 1;
        matrix[1][2] = 0;

        matrix[2][0] = s;
        matrix[2][1] = 0;
        matrix[2][2] = c;
        return *this;
    }

    template<typename T>
    inline matrix33<T>& matrix33<T>::loadRotationZ(T angle)
    {
        const T c = cos(angle);
        const T s = sin(angle);

        matrix[0][0] = c;
        matrix[0][1] = s;
        matrix[0][2] = 0;

        matrix[1][0] = -s;
        matrix[1][1] = c;
        matrix[1][2] = 0;

        matrix[2][0] = 0;
        matrix[2][1] = 0;
        matrix[2][2] = 1;
        return *this;
    }

    template<typename T>
    inline matrix33<T>& matrix33<T>::invert()
    {
        const T det = determinant();
        if (fabs(det) < VVV_EPSILON)
            throw std::logic_error("near zero determinant in matrix inversion");

        const T invDet = 1.0 / det;

        const T m00 = (matrix[1][1] * matrix[2][2] - matrix[2][1] * matrix[1][2]) * invDet;
        const T m10 = -(matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) * invDet;
        const T m20 = (matrix[1][0] * matrix[2][1] - matrix[2][0] * matrix[1][1]) * invDet;

        const T m01 = -(matrix[0][1] * matrix[2][2] - matrix[0][2] * matrix[2][1]) * invDet;
        const T m11 = (matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]) * invDet;
        const T m21 = -(matrix[0][0] * matrix[2][1] - matrix[2][0] * matrix[0][1]) * invDet;

        const T m02 = (matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]) * invDet;
        const T m12 = -(matrix[0][0] * matrix[1][2] - matrix[1][0] * matrix[0][2]) * invDet;
        const T m22 = (matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1]) * invDet;

        matrix[0][0] = m00;
        matrix[1][0] = m10;
        matrix[2][0] = m20;

        matrix[0][1] = m01;
        matrix[1][1] = m11;
        matrix[2][1] = m21;

        matrix[0][2] = m02;
        matrix[1][2] = m12;
        matrix[2][2] = m22;

        return *this;
    }

    template<typename T>
    inline void matrix33<T>::mul(const matrix33<T>& lhs, const matrix33<T>& rhs, matrix33<T>& out)
    {
        const T x0 = lhs.matrix[0][0] * rhs.matrix[0][0] + lhs.matrix[1][0] * rhs.matrix[0][1] + lhs.matrix[2][0] * rhs.matrix[0][2];
        const T x1 = lhs.matrix[0][1] * rhs.matrix[0][0] + lhs.matrix[1][1] * rhs.matrix[0][1] + lhs.matrix[2][1] * rhs.matrix[0][2];
        const T x2 = lhs.matrix[0][2] * rhs.matrix[0][0] + lhs.matrix[1][2] * rhs.matrix[0][1] + lhs.matrix[2][2] * rhs.matrix[0][2];
        const T x3 = lhs.matrix[0][0] * rhs.matrix[1][0] + lhs.matrix[1][0] * rhs.matrix[1][1] + lhs.matrix[2][0] * rhs.matrix[1][2];
        const T x4 = lhs.matrix[0][1] * rhs.matrix[1][0] + lhs.matrix[1][1] * rhs.matrix[1][1] + lhs.matrix[2][1] * rhs.matrix[1][2];
        const T x5 = lhs.matrix[0][2] * rhs.matrix[1][0] + lhs.matrix[1][2] * rhs.matrix[1][1] + lhs.matrix[2][2] * rhs.matrix[1][2];
        const T x6 = lhs.matrix[0][0] * rhs.matrix[2][0] + lhs.matrix[1][0] * rhs.matrix[2][1] + lhs.matrix[2][0] * rhs.matrix[2][2];
        const T x7 = lhs.matrix[0][1] * rhs.matrix[2][0] + lhs.matrix[1][1] * rhs.matrix[2][1] + lhs.matrix[2][1] * rhs.matrix[2][2];
        const T x8 = lhs.matrix[0][2] * rhs.matrix[2][0] + lhs.matrix[1][2] * rhs.matrix[2][1] + lhs.matrix[2][2] * rhs.matrix[2][2];
        out.matrix[0][0] = x0;
        out.matrix[0][1] = x1;
        out.matrix[0][2] = x2;
        out.matrix[1][0] = x3;
        out.matrix[1][1] = x4;
        out.matrix[1][2] = x5;
        out.matrix[2][0] = x6;
        out.matrix[2][1] = x7;
        out.matrix[2][2] = x8;
    }

    template<typename T>
    inline matrix33<T> matrix33<T>::createMatrixFromColumns(const vector3<T>& v0, const vector3<T>& v1, const vector3<T>& v2)
    {
        matrix33<T> ret;
        ret.matrix[0][0] = v0.x;
        ret.matrix[0][1] = v0.y;
        ret.matrix[0][2] = v0.z;

        ret.matrix[1][0] = v1.x;
        ret.matrix[1][1] = v1.y;
        ret.matrix[1][2] = v1.z;

        ret.matrix[2][0] = v2.x;
        ret.matrix[2][1] = v2.y;
        ret.matrix[2][2] = v2.z;

        return ret;
    }

    template<typename T>
    inline matrix33<T> matrix33<T>::createMatrixFromRows(const vector3<T>& v0, const vector3<T>& v1, const vector3<T>& v2)
    {
        matrix33<T> ret;
        ret.matrix[0][0] = v0.x;
        ret.matrix[1][0] = v0.y;
        ret.matrix[2][0] = v0.z;

        ret.matrix[0][1] = v1.x;
        ret.matrix[1][1] = v1.y;
        ret.matrix[2][1] = v1.z;

        ret.matrix[0][2] = v2.x;
        ret.matrix[1][2] = v2.y;
        ret.matrix[2][2] = v2.z;

        return ret;
    }


    template<typename T>
    inline T matrix44<T>::determinant() const
    {
        const T v00 = matrix[0][0];
        const T v01 = matrix[0][1];
        const T v02 = matrix[0][2];
        const T v03 = matrix[0][3];

        const T v10 = matrix[1][0];
        const T v11 = matrix[1][1];
        const T v12 = matrix[1][2];
        const T v13 = matrix[1][3];

        const T v20 = matrix[2][0];
        const T v21 = matrix[2][1];
        const T v22 = matrix[2][2];
        const T v23 = matrix[2][3];

        const T v30 = matrix[3][0];
        const T v31 = matrix[3][1];
        const T v32 = matrix[3][2];
        const T v33 = matrix[3][3];

        const T sf00 = v22 * v33 - v32*v23;
        const T sf04 = v21 * v33 - v31*v23;
        const T sf08 = v21 * v32 - v31*v22;
        const T sf12 = v20 * v33 - v30*v23;
        const T sf16 = v20 * v32 - v30*v22;
        const T sf20 = v20 * v31 - v30*v21;

        const T inv00 = v11 * sf00 - v12 * sf04 + v13 * sf08;
        const T inv10 = -(v10 * sf00 - v12 * sf12 + v13 * sf16);
        const T inv20 = v10 * sf04 - v11 * sf12 + v13 * sf20;
        const T inv30 = -(v10 * sf08 - v11 * sf16 + v12 * sf20);

        return v00 * inv00 + v01 * inv10 + v02 * inv20 + v03*inv30;
    }

    template<typename T>
    inline matrix44<T>& matrix44<T>::invert()
    {
        const T v00 = matrix[0][0];
        const T v01 = matrix[0][1];
        const T v02 = matrix[0][2];
        const T v03 = matrix[0][3];

        const T v10 = matrix[1][0];
        const T v11 = matrix[1][1];
        const T v12 = matrix[1][2];
        const T v13 = matrix[1][3];

        const T v20 = matrix[2][0];
        const T v21 = matrix[2][1];
        const T v22 = matrix[2][2];
        const T v23 = matrix[2][3];

        const T v30 = matrix[3][0];
        const T v31 = matrix[3][1];
        const T v32 = matrix[3][2];
        const T v33 = matrix[3][3];

        const T sf00 = v22 * v33 - v32*v23;
        const T sf02 = v12 * v33 - v32*v13;
        const T sf03 = v12 * v23 - v22*v13;

        const T sf04 = v21 * v33 - v31*v23;
        const T sf06 = v11 * v33 - v31*v13;
        const T sf07 = v11 * v23 - v21*v13;

        const T sf08 = v21 * v32 - v31*v22;
        const T sf10 = v11 * v32 - v31*v12;
        const T sf11 = v11 * v22 - v21*v12;

        const T sf12 = v20 * v33 - v30*v23;
        const T sf14 = v10 * v33 - v30*v13;
        const T sf15 = v10 * v23 - v20*v13;

        const T sf16 = v20 * v32 - v30*v22;
        const T sf18 = v10 * v32 - v30*v12;
        const T sf19 = v10 * v22 - v20*v12;

        const T sf20 = v20 * v31 - v30*v21;
        const T sf22 = v10 * v31 - v30*v11;
        const T sf23 = v10 * v21 - v20*v11;

        const T inv00 = v11 * sf00 - v12 * sf04 + v13 * sf08;
        const T inv10 = -(v10 * sf00 - v12 * sf12 + v13 * sf16);
        const T inv20 = v10 * sf04 - v11 * sf12 + v13 * sf20;
        const T inv30 = -(v10 * sf08 - v11 * sf16 + v12 * sf20);

        const T det = v00 * inv00 + v01 * inv10 + v02 * inv20 + v03*inv30;

        if( fabs(det) < VVV_EPSILON )
        {
            throw std::logic_error("near zero determinant in matrix inversion");
        }

        if( fabs(1.0f - det) <= VVV_EPSILON )
        {
            /*
             * This branch was made because in 3d determinant often near 1.0
             * so this condition prevents multiplying of each element on 1/det
             */
            matrix[0][0] = inv00;
            matrix[0][1] = -(v01 * sf00 - v02 * sf04 + v03 * sf08);
            matrix[0][2] = v01 * sf02 - v02 * sf06 + v03*sf10;
            matrix[0][3] = -(v01 * sf03 - v02 * sf07 + v03 * sf11);

            matrix[1][0] = inv10;
            matrix[1][1] = v00 * sf00 - v02 * sf12 + v03*sf16;
            matrix[1][2] = -(v00 * sf02 - v02 * sf14 + v03 * sf18);
            matrix[1][3] = v00 * sf03 - v02 * sf15 + v03*sf19;

            matrix[2][0] = inv20;
            matrix[2][1] = -(v00 * sf04 - v01 * sf12 + v03 * sf20);
            matrix[2][2] = v00 * sf06 - v01 * sf14 + v03*sf22;
            matrix[2][3] = -(v00 * sf07 - v01 * sf15 + v03 * sf23);

            matrix[3][0] = inv30;
            matrix[3][1] = v00 * sf08 - v01 * sf16 + v02*sf20;
            matrix[3][2] = -(v00 * sf10 - v01 * sf18 + v02 * sf22);
            matrix[3][3] = v00 * sf11 - v01 * sf19 + v02*sf23;
        }
        else
        {
            const T invdet = 1.0 / det;
            matrix[0][0] = inv00 * invdet;
            matrix[0][1] = -(v01 * sf00 - v02 * sf04 + v03 * sf08) * invdet;
            matrix[0][2] = (v01 * sf02 - v02 * sf06 + v03 * sf10) * invdet;
            matrix[0][3] = -(v01 * sf03 - v02 * sf07 + v03 * sf11) * invdet;

            matrix[1][0] = inv10 * invdet;
            matrix[1][1] = (v00 * sf00 - v02 * sf12 + v03 * sf16) * invdet;
            matrix[1][2] = -(v00 * sf02 - v02 * sf14 + v03 * sf18) * invdet;
            matrix[1][3] = (v00 * sf03 - v02 * sf15 + v03 * sf19) * invdet;

            matrix[2][0] = inv20 * invdet;
            matrix[2][1] = -(v00 * sf04 - v01 * sf12 + v03 * sf20) * invdet;
            matrix[2][2] = (v00 * sf06 - v01 * sf14 + v03 * sf22) * invdet;
            matrix[2][3] = -(v00 * sf07 - v01 * sf15 + v03 * sf23) * invdet;

            matrix[3][0] = inv30 * invdet;
            matrix[3][1] = (v00 * sf08 - v01 * sf16 + v02 * sf20) * invdet;
            matrix[3][2] = -(v00 * sf10 - v01 * sf18 + v02 * sf22) * invdet;
            matrix[3][3] = (v00 * sf11 - v01 * sf19 + v02 * sf23) * invdet;
        }

        return *this;
    }

    template<typename T>
    inline void matrix44<T>::mul(const matrix44& lhs, const matrix44& rhs, matrix44<T>& out)
    {
        const T out00 = lhs.matrix[0][0] * rhs.matrix[0][0] + lhs.matrix[1][0] * rhs.matrix[0][1] + lhs.matrix[2][0] * rhs.matrix[0][2] + lhs.matrix[3][0] * rhs.matrix[0][3];
        const T out01 = lhs.matrix[0][1] * rhs.matrix[0][0] + lhs.matrix[1][1] * rhs.matrix[0][1] + lhs.matrix[2][1] * rhs.matrix[0][2] + lhs.matrix[3][1] * rhs.matrix[0][3];
        const T out02 = lhs.matrix[0][2] * rhs.matrix[0][0] + lhs.matrix[1][2] * rhs.matrix[0][1] + lhs.matrix[2][2] * rhs.matrix[0][2] + lhs.matrix[3][2] * rhs.matrix[0][3];
        const T out03 = lhs.matrix[0][3] * rhs.matrix[0][0] + lhs.matrix[1][3] * rhs.matrix[0][1] + lhs.matrix[2][3] * rhs.matrix[0][2] + lhs.matrix[3][3] * rhs.matrix[0][3];

        const T out10 = lhs.matrix[0][0] * rhs.matrix[1][0] + lhs.matrix[1][0] * rhs.matrix[1][1] + lhs.matrix[2][0] * rhs.matrix[1][2] + lhs.matrix[3][0] * rhs.matrix[1][3];
        const T out11 = lhs.matrix[0][1] * rhs.matrix[1][0] + lhs.matrix[1][1] * rhs.matrix[1][1] + lhs.matrix[2][1] * rhs.matrix[1][2] + lhs.matrix[3][1] * rhs.matrix[1][3];
        const T out12 = lhs.matrix[0][2] * rhs.matrix[1][0] + lhs.matrix[1][2] * rhs.matrix[1][1] + lhs.matrix[2][2] * rhs.matrix[1][2] + lhs.matrix[3][2] * rhs.matrix[1][3];
        const T out13 = lhs.matrix[0][3] * rhs.matrix[1][0] + lhs.matrix[1][3] * rhs.matrix[1][1] + lhs.matrix[2][3] * rhs.matrix[1][2] + lhs.matrix[3][3] * rhs.matrix[1][3];

        const T out20 = lhs.matrix[0][0] * rhs.matrix[2][0] + lhs.matrix[1][0] * rhs.matrix[2][1] + lhs.matrix[2][0] * rhs.matrix[2][2] + lhs.matrix[3][0] * rhs.matrix[2][3];
        const T out21 = lhs.matrix[0][1] * rhs.matrix[2][0] + lhs.matrix[1][1] * rhs.matrix[2][1] + lhs.matrix[2][1] * rhs.matrix[2][2] + lhs.matrix[3][1] * rhs.matrix[2][3];
        const T out22 = lhs.matrix[0][2] * rhs.matrix[2][0] + lhs.matrix[1][2] * rhs.matrix[2][1] + lhs.matrix[2][2] * rhs.matrix[2][2] + lhs.matrix[3][2] * rhs.matrix[2][3];
        const T out23 = lhs.matrix[0][3] * rhs.matrix[2][0] + lhs.matrix[1][3] * rhs.matrix[2][1] + lhs.matrix[2][3] * rhs.matrix[2][2] + lhs.matrix[3][3] * rhs.matrix[2][3];

        const T out30 = lhs.matrix[0][0] * rhs.matrix[3][0] + lhs.matrix[1][0] * rhs.matrix[3][1] + lhs.matrix[2][0] * rhs.matrix[3][2] + lhs.matrix[3][0] * rhs.matrix[3][3];
        const T out31 = lhs.matrix[0][1] * rhs.matrix[3][0] + lhs.matrix[1][1] * rhs.matrix[3][1] + lhs.matrix[2][1] * rhs.matrix[3][2] + lhs.matrix[3][1] * rhs.matrix[3][3];
        const T out32 = lhs.matrix[0][2] * rhs.matrix[3][0] + lhs.matrix[1][2] * rhs.matrix[3][1] + lhs.matrix[2][2] * rhs.matrix[3][2] + lhs.matrix[3][2] * rhs.matrix[3][3];
        const T out33 = lhs.matrix[0][3] * rhs.matrix[3][0] + lhs.matrix[1][3] * rhs.matrix[3][1] + lhs.matrix[2][3] * rhs.matrix[3][2] + lhs.matrix[3][3] * rhs.matrix[3][3];

        out.matrix[0][0] = out00;
        out.matrix[0][1] = out01;
        out.matrix[0][2] = out02;
        out.matrix[0][3] = out03;

        out.matrix[1][0] = out10;
        out.matrix[1][1] = out11;
        out.matrix[1][2] = out12;
        out.matrix[1][3] = out13;

        out.matrix[2][0] = out20;
        out.matrix[2][1] = out21;
        out.matrix[2][2] = out22;
        out.matrix[2][3] = out23;

        out.matrix[3][0] = out30;
        out.matrix[3][1] = out31;
        out.matrix[3][2] = out32;
        out.matrix[3][3] = out33;
    }

    template<typename T>
    inline matrix44<T>& matrix44<T>::loadRotation(T angle, T ax, T ay, T az)
    {
        const T c = cos(angle);
        const T s = sin(angle);
        const T axisLength = vector3<T>::length(ax, ay, az);

        if( axisLength != 0 )
        {
            ax /= axisLength;
            ay /= axisLength;
            az /= axisLength;
        }

        const T axcm = (1.0 - c) * ax;
        const T azcm = (1.0 - c) * az;

        matrix[0][0] = c + axcm*ax;
        matrix[0][1] = axcm * ay + s*az;
        matrix[0][2] = axcm * az - s*ay;
        matrix[0][3] = 0;

        matrix[1][0] = axcm * ay - s*az;
        matrix[1][1] = c + (1.0f - c) * ay*ay;
        matrix[1][2] = azcm * ay + s*ax;
        matrix[1][3] = 0;

        matrix[2][0] = axcm * az + s*ay;
        matrix[2][1] = azcm * ay - s*ax;
        matrix[2][2] = c + azcm*az;
        matrix[2][3] = 0;

        matrix[3][0] = 0;
        matrix[3][1] = 0;
        matrix[3][2] = 0;
        matrix[3][3] = 1;
        return *this;
    }

    template<typename T>
    inline matrix44<T>& matrix44<T>::loadRotationX(T angle)
    {
        const T c = cos(angle);
        const T s = sin(angle);

        matrix[0][0] = 1;
        matrix[0][1] = 0;
        matrix[0][2] = 0;
        matrix[0][3] = 0;

        matrix[1][0] = 0;
        matrix[1][1] = c;
        matrix[1][2] = s;
        matrix[1][3] = 0;

        matrix[2][0] = 0;
        matrix[2][1] = -s;
        matrix[2][2] = c;
        matrix[2][3] = 0;

        matrix[3][0] = 0;
        matrix[3][1] = 0;
        matrix[3][2] = 0;
        matrix[3][3] = 1;
        return *this;
    }

    template<typename T>
    inline matrix44<T>& matrix44<T>::loadRotationY(T angle)
    {
        const T c = cos(angle);
        const T s = sin(angle);

        matrix[0][0] = c;
        matrix[0][1] = 0;
        matrix[0][2] = -s;
        matrix[0][3] = 0;

        matrix[1][0] = 0;
        matrix[1][1] = 1;
        matrix[1][2] = 0;
        matrix[1][3] = 0;

        matrix[2][0] = s;
        matrix[2][1] = 0;
        matrix[2][2] = c;
        matrix[2][3] = 0;

        matrix[3][0] = 0;
        matrix[3][1] = 0;
        matrix[3][2] = 0;
        matrix[3][3] = 1;
        return *this;
    }

    template<typename T>
    inline matrix44<T>& matrix44<T>::loadRotationZ(T angle)
    {
        const T c = cos(angle);
        const T s = sin(angle);

        matrix[0][0] = c;
        matrix[0][1] = s;
        matrix[0][2] = 0;
        matrix[0][3] = 0;

        matrix[1][0] = -s;
        matrix[1][1] = c;
        matrix[1][2] = 0;
        matrix[1][3] = 0;

        matrix[2][0] = 0;
        matrix[2][1] = 0;
        matrix[2][2] = 1;
        matrix[2][3] = 0;

        matrix[3][0] = 0;
        matrix[3][1] = 0;
        matrix[3][2] = 0;
        matrix[3][3] = 1;
        return *this;
    }

    template<typename T>
    inline matrix44<T>& matrix44<T>::loadTranslation(const vector3<T> &to)
    {
        matrix[0][0] = 1;
        matrix[0][1] = 0;
        matrix[0][2] = 0;
        matrix[0][3] = 0;

        matrix[1][0] = 0;
        matrix[1][1] = 1;
        matrix[1][2] = 0;
        matrix[1][3] = 0;

        matrix[2][0] = 0;
        matrix[2][1] = 0;
        matrix[2][2] = 1;
        matrix[2][3] = 0;

        matrix[3][0] = to.x;
        matrix[3][1] = to.y;
        matrix[3][2] = to.z;
        matrix[3][3] = 1;
        return *this;
    }

    template<typename T>
    matrix44<T>& matrix44<T>::loadScale(const T& x, const T& y, const T& z)
    {
        matrix[0][0] = x;
        matrix[0][1] = 0;
        matrix[0][2] = 0;
        matrix[0][3] = 0;

        matrix[1][0] = 0;
        matrix[1][1] = y;
        matrix[1][2] = 0;
        matrix[1][3] = 0;

        matrix[2][0] = 0;
        matrix[2][1] = 0;
        matrix[2][2] = z;
        matrix[2][3] = 0;

        matrix[3][0] = 0;
        matrix[3][1] = 0;
        matrix[3][2] = 0;
        matrix[3][3] = 1;
        return *this;
    }

    template<typename T>
    matrix44<T>& matrix44<T>::loadScale(const vector3<T>& s)
    {
        matrix[0][0] = s.x;
        matrix[0][1] = 0;
        matrix[0][2] = 0;
        matrix[0][3] = 0;

        matrix[1][0] = 0;
        matrix[1][1] = s.y;
        matrix[1][2] = 0;
        matrix[1][3] = 0;

        matrix[2][0] = 0;
        matrix[2][1] = 0;
        matrix[2][2] = s.z;
        matrix[2][3] = 0;

        matrix[3][0] = 0;
        matrix[3][1] = 0;
        matrix[3][2] = 0;
        matrix[3][3] = 1;
        return *this;
    }

    template<typename T>
    inline matrix44<T>& matrix44<T>::loadTranslation(const T& x, const T& y, const T& z )
    {
        return loadTranslation( vector3<T>(x,y,z) );
    }

    template<typename T>
    inline vector4<T> matrix44<T>::transform(const vector4<T>& in) const
    {
        const T x = matrix[0][0] * in.x + matrix[1][0] * in.y + matrix[2][0] * in.z + matrix[3][0] * in.w;
        const T y = matrix[0][1] * in.x + matrix[1][1] * in.y + matrix[2][1] * in.z + matrix[3][1] * in.w;
        const T z = matrix[0][2] * in.x + matrix[1][2] * in.y + matrix[2][2] * in.z + matrix[3][2] * in.w;
        const T w = matrix[0][3] * in.x + matrix[1][3] * in.y + matrix[2][3] * in.z + matrix[3][3] * in.w;
        return vector4<T>(x, y, z, w);
    }

    template<typename T>
    inline void matrix44<T>::transform(const vector4<T>& in, vector4<T>& out) const
    {
        const T x = matrix[0][0] * in.x + matrix[1][0] * in.y + matrix[2][0] * in.z + matrix[3][0] * in.w;
        const T y = matrix[0][1] * in.x + matrix[1][1] * in.y + matrix[2][1] * in.z + matrix[3][1] * in.w;
        const T z = matrix[0][2] * in.x + matrix[1][2] * in.y + matrix[2][2] * in.z + matrix[3][2] * in.w;
        const T w = matrix[0][3] * in.x + matrix[1][3] * in.y + matrix[2][3] * in.z + matrix[3][3] * in.w;
        out.set(x, y, z, w);
    }

    template<typename T>
    inline void matrix44<T>::transform( const vector3<T>& in, vector3<T>& out) const
    {
        const T x = matrix[0][0] * in.x + matrix[1][0] * in.y + matrix[2][0] * in.z + matrix[3][0];
        const T y = matrix[0][1] * in.x + matrix[1][1] * in.y + matrix[2][1] * in.z + matrix[3][1];
        const T z = matrix[0][2] * in.x + matrix[1][2] * in.y + matrix[2][2] * in.z + matrix[3][2];
        out.set(x, y, z);
    }

    template<typename T>
    inline matrix33<T> matrix44<T>::getRotationPart() const
    {
        matrix33<T> ret;
        for(size_t i = 0; i < 3; ++i)
            for(size_t j = 0; j < 3; ++j)
                ret.matrix[i][j] = this->matrix[i][j];
        return ret;
    }

    template<typename T>
    inline vector3<T> matrix44<T>::transform(const vector3<T> &in) const
    {
        const T x = matrix[0][0] * in.x + matrix[1][0] * in.y + matrix[2][0] * in.z + matrix[3][0];
        const T y = matrix[0][1] * in.x + matrix[1][1] * in.y + matrix[2][1] * in.z + matrix[3][1];
        const T z = matrix[0][2] * in.x + matrix[1][2] * in.y + matrix[2][2] * in.z + matrix[3][2];
        return vector3<T>( x, y, z );
    }


    template<typename T>
    inline matrix44<T>& matrix44<T>::loadFromColumns(const vector4<T> &v0, const vector4<T> &v1, const vector4<T> &v2, const vector4<T> &v3)
    {
        matrix[0][0] = v0.x;
        matrix[0][1] = v0.y;
        matrix[0][2] = v0.z;
        matrix[0][3] = v0.w;

        matrix[1][0] = v1.x;
        matrix[1][1] = v1.y;
        matrix[1][2] = v1.z;
        matrix[1][3] = v1.w;

        matrix[2][0] = v2.x;
        matrix[2][1] = v2.y;
        matrix[2][2] = v2.z;
        matrix[2][3] = v2.w;

        matrix[3][0] = v3.x;
        matrix[3][1] = v3.y;
        matrix[3][2] = v3.z;
        matrix[3][3] = v3.w;
        return *this;

    }

    template<typename T>
    inline matrix44<T>& matrix44<T>::loadLookAt(const vector3<T>& eye, const vector3<T>& target, const  vector3<T>& up)
    {
        const vector3<T> zaxis( (eye - target ).normalize() );
        const vector3<T> xaxis( up.cross(zaxis).normalize() );
        const vector3<T> yaxis(      zaxis.cross(xaxis)     );

        return loadFromColumns( vector4<T>(         xaxis.x,         yaxis.x,         zaxis.x, 0),
                                vector4<T>(         xaxis.y,         yaxis.y,         zaxis.y, 0),
                                vector4<T>(         xaxis.z,         yaxis.z,         zaxis.z, 0),
                                vector4<T>( -xaxis.dot(eye), -yaxis.dot(eye), -zaxis.dot(eye), 1) );
    }


    template<typename T>
    inline void quaternion<T>::toMatrix33( matrix33<T>& mat) const
    {
        mat.matrix[0][0] = 1 - 2 * q.y * q.y  - 2 * q.z * q.z;
        mat.matrix[0][1] = 2 * q.x * q.y + 2 * q.w * q.z;
        mat.matrix[0][2] = 2 * q.x * q.z - 2 * q.w * q.y;

        mat.matrix[1][0] = 2 * q.x * q.y - 2 * q.w * q.z;
        mat.matrix[1][1] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
        mat.matrix[1][2] = 2 * q.y * q.z + 2 * q.w * q.x;

        mat.matrix[2][0] = 2 * q.x * q.z + 2 * q.w * q.y;
        mat.matrix[2][1] = 2 * q.y * q.z - 2 * q.w * q.x;
        mat.matrix[2][2] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
   }

    template<typename T>
    inline void quaternion<T>::toMatrix44( matrix44<T>& mat) const
    {
        mat.matrix[0][0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
        mat.matrix[0][1] = 2 * q.x * q.y + 2 * q.w * q.z;
        mat.matrix[0][2] = 2 * q.x * q.z - 2 * q.w * q.y;
        mat.matrix[0][3] = 0;

        mat.matrix[1][0] = 2 * q.x * q.y - 2 * q.w * q.z;
        mat.matrix[1][1] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
        mat.matrix[1][2] = 2 * q.y * q.z + 2 * q.w * q.x;
        mat.matrix[1][3] = 0;

        mat.matrix[2][0] = 2 * q.x * q.z + 2 * q.w * q.y;
        mat.matrix[2][1] = 2 * q.y * q.z - 2 * q.w * q.x;
        mat.matrix[2][2] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
        mat.matrix[2][3] = 0;

        mat.matrix[3][0] = 0;
        mat.matrix[3][1] = 0;
        mat.matrix[3][2] = 0;
        mat.matrix[3][3] = 1;
    }
}


#endif // LINALG_H
