#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <cfloat>
#include <tr1/functional>

template<int N, typename T=double>
class poly
{
    public:
        poly()
        {
            memset(coef, 0, sizeof(coef) );
        }

        /**
          src - [an, ..., a3, a2, a1]
                an*x^n + ... + a3*x^2 +  a2*x + a1  */
        poly(const T *src)
        {
           for(int i = 0; i < N; ++i)
           {
               coef[i] = src[N-i-1];
           }
        }

        /**
          src - [an, ..., a3, a2, a1]
                an*x^n + ... + a3*x^2 +  a2*x + a1  */
        poly(T an, ... )
        {
            va_list args;
            va_start(args,an);
            coef[N-1] = an;
            for(int i=N-2; i >= 0; --i)
            {
                coef[i] = va_arg(args, T );
            }
            va_end(args);
        }

        /** Calculate polinomial value of argument val */
        T calc(const T& val) const
        {
            T res = coef[N-1];
            for( int i=N-2; i >= 0; --i)
            {
                res = res*val + coef[i];
            }

            return res;
        }

        /** Generate derivative of polinomial. Result new polinomial of degree N-1 */
        poly<N-1, T> genDeriv() const
        {
            // [c,b,a] -> der[2a,b]->[b,2a]
            T der[N];
            for(int i =0; i < N; ++i)
            {
                der[i] = coef[N-i-1]*(N-i-1);
            }
            return poly<N-1,T>(der);
        }

        void print() const
        {
            std::cout << "{ ";
            for(int i = N-1; i >= 0; --i)
            {
                std::cout << coef[i] << " ";
            }
            std::cout << "}" << std::endl;
        }

        /** Find root of equation P(x)=0 with first aproximation of x0
            and wanted precision of epsilon */
        T solveLocal(const T& x0, T epsilon=0.000001) const
        {
            const poly<N-1,T> d = genDeriv();
            epsilon = fabs(epsilon);
            T ret;
            T prev = x0;
            while(true)
            {
                ret = prev - calc(prev) / d.calc(prev);
                if( fabs( ret-prev ) < epsilon )
                    break;
                prev = ret;
            }
            return ret;
        }


        /** Find root of equation P(x)=0 with first aproximation of x0.
            Do NUM_ITERATIONS iterations. */
        template<int NUM_ITERATIONS>
        T solveLocal(T x0) const
        {
            const poly<N-1,T> d = genDeriv();
            T ret = x0;
            for(int i=0; i < NUM_ITERATIONS; ++i)
            {
                ret = ret - calc(ret)/d.calc(ret);
            }
            return ret;
        }

        poly<N,T>& operator+=(T v)
        {
            coef[0] += v;
            return *this;
        }

        poly<N,T> operator+(T v) const
        {
            poly<N,T> ret( *this );
            ret += v;
            return ret;
        }

        poly<N,T>& operator-=(T v)
        {
            coef[0] -= v;
            return *this;
        }

        poly<N,T> operator-(T v) const
        {
            poly<N> ret( *this );
            ret -= v;
            return ret;
        }

        poly<N,T>& operator*=(T v)
        {
            for(int i =0; i < N; ++i)
            {
                coef[i] *= v;
            }
            return *this;
        }

        poly<N,T> operator*(T v) const
        {
            poly<N> ret(*this);
            ret *=v;
            return ret;
        }

        poly<N,T>& operator/=(T v)
        {
            for(int i =0; i < N; ++i)
            {
                coef[i] /= v;
            }
            return *this;
        }

        poly<N,T> operator/(T v) const
        {
            poly<N,T> ret(*this);
            ret /= v;
            return ret;
        }


        template<int N1>
        poly<(N>N1)?N:N1 , T> operator+(const poly<N1>& p) const
        {
            poly<(N>N1)?N:N1, T> ret;
            for(int i=0; i < ((N>N1)?N1:N); ++i)
            {
                ret.coef[i] = this->coef[i] + p.coef[i];
            }
            if(N>N1)
                for( int i=N1; i < N; ++i)
                {
                    ret.coef[i] = coef[i];
                }
            else
                for( int i=N; i < N1; ++i)
                {
                    ret.coef[i] = p.coef[i];
                }

            return ret;
        }

        template<int N1>
        poly<(N>N1)?N:N1, T> operator-(const poly<N1>& p) const
        {
            poly<(N>N1)?N:N1, T> ret;
            for(int i=0; i < ((N>N1)?N1:N); ++i)
            {
                ret.coef[i] = this->coef[i] - p.coef[i];
            }
            if(N>N1)
                for( int i=N1; i < N; ++i)
                {
                    ret.coef[i] = coef[i];
                }
            else
                for( int i=N; i < N1; ++i)
                {
                    ret.coef[i] = p.coef[i];
                }

            return ret;
        }


        template<int N1>
        poly<N+N1-1, T > operator*(const poly<N1,T>& p) const
        {
            poly<N+N1-1,T> ret;
            for(int i =0; i < N; ++i)
            {
                for( int j = 0; j < N1; ++j)
                {
                    ret.coef[i+j] += coef[i] * p.coef[j];
                }
            }
            return ret;
        }

        //     ax+b  -> [b,a]
        // ax2+bx+c  -> [c,b,a]
        T coef[N];
};

template<int N, typename T >
inline poly<N,T> operator+(T v, const poly<N>& p)
{
    return p + v;
}

template<int N, typename T >
inline poly<N,T> operator-(T v, const poly<N>& p)
{
    return p - v;
}

template<int N, typename T >
inline poly<N,T> operator*(T v, const poly<N>& p)
{
    return p * v;
}

template<int N, typename T >
inline poly<N,T> operator/(T v, const poly<N>& p)
{
    return p / v;
}

/** Find polinomial fit given 3 points.
    @param x - array of arguments
    @param y - array of corresponding values */
template< typename T >
inline poly<3,T> poly3fit( const double* x, const T* y )
{
    const double x00 = x[0]*x[0];
    const double x01 = x[0]*x[1];
    const double x02 = x[0]*x[2];
    const double x11 = x[1]*x[1];
    const double x12 = x[1]*x[2];
    const double x22 = x[2]*x[2];
    const double x00mmp = x00-x01-x02+x12;
    const double x11mpm = x11-x01+x02-x12;
    const double x22pmm = x22+x01-x02-x12;

    if( x00mmp == 0 || x11mpm == 0 || x22pmm==0 )
        return poly<3,T>(DBL_MAX,DBL_MAX,DBL_MAX);

    const T y0 = y[0] / x00mmp;
    const T y1 = y[1] / x11mpm;
    const T y2 = y[2] / x22pmm;

    const T o2 = y0 + y1 + y2;
    const T o1 =   -(x[1]+x[2])*y0
                        -(x[0]+x[2])*y1
                        -(x[0]+x[1])*y2 ;
    const T o0 = x12*y0 + x02*y1 + x01*y2;

    return poly<3,T>(o2,o1,o0);
}

/** Find polinomial fit given 5 points.
    @param x - array of arguments
    @param y - array of corresponding values */
template<typename T>
inline poly<5,T> poly5fit(const double* x, const T* y)
{
    const double x0 = x[0];
    const double x1 = x[1];
    const double x2 = x[2];
    const double x3 = x[3];
    const double x4 = x[4];
    const double sx01234 = x0+x1+x2+x3+x4;
    const T y0 = y[0] / ( (x0-x1)*(x0-x2)*(x0-x3)*(x0-x4) );
    const T y1 = y[1] / ( (x1-x0)*(x1-x2)*(x1-x3)*(x1-x4) );
    const T y2 = y[2] / ( (x2-x0)*(x2-x1)*(x2-x3)*(x2-x4) );
    const T y3 = y[3] / ( (x3-x0)*(x3-x1)*(x3-x2)*(x3-x4) );
    const T y4 = y[4] / ( (x4-x0)*(x4-x1)*(x4-x2)*(x4-x3) );

    const double sx34 = x3+x4;
    const double mx34 = x3*x4;
    const double sx01 = x0+x1;
    const double sx24 = x2+x4;
    const double sx23 = x2+x3;


    const T o4 = y0 + y1 + y2 + y3 + y4;

    const T o3 = -(y0*(sx01234-x0) + y1*(sx01234-x1) +y2*(sx01234-x2) +y3*(sx01234-x3) +y4*(sx01234-x4) );

    const T o2 =  y0*( x1*x2+sx34*(x1 + x2) + mx34 )
                +y1*( x0*x2+sx34*(x0 + x2) + mx34 )
                +y2*( x0*x1+sx34*sx01 + mx34 )
                +y3*( x0*x1+sx24*sx01 + x2*x4 )
                +y4*( x0*x1+sx23*sx01 + x2*x3 );
    const T o1 = - (  y0*(x1*x2*sx34 + mx34*(x1+x2) )
                +y1*(x0*x2*sx34 + mx34*(x0+x2) )
                +y2*(x0*x1*sx34 + mx34*sx01 )
                +y3*(x0*x1*sx24 + x2*x4*sx01 )
                +y4*(x0*x1*sx23 + x2*x3*sx01 )   );

    const T o0 = y0*x1*x2*x3*x4 + y1*x0*x2*x3*x4 +y2*x0*x1*x3*x4 +y3*x0*x1*x2*x4 +y4*x0*x1*x2*x3;

    return poly<5,T>(o4,o3,o2,o1,o0);
}


template<typename T>
inline poly<5,T> poly5fit( const std::tr1::function<T(double)>& f, double start, double end )
{
    const double len_4 = (end-start) / 4;
    double xx[5];
    T yy[5];

    for( size_t i = 0; i < 5; ++i)
    {
        xx[i] = start + i*len_4;
    }

    for( size_t i = 0; i < 5; ++i)
    {
        yy[i] = f(xx[i]);
    }

    return poly5fit<T>(xx,yy);
}

template<typename T>
inline poly<3,T> poly3fit( const std::tr1::function<T(double)>& f, double start, double end )
{
    const double    len_2 = (end-start) / 2;
    double          xx[3];
    T yy[3];

    for( size_t i = 0; i < 3; ++i)
    {
        xx[i] = start + i*len_2;
    }

    for( size_t i = 0; i < 3; ++i)
    {
        yy[i] = f( xx[i] );
    }

    return poly3fit<T>(xx,yy);
}

#endif // POLYNOMIAL_H
