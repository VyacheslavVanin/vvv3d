#ifndef MATRICES_H
#define MATRICES_H

#define M_PIf static_cast<float>(M_PI)
namespace vvv
{
template<typename T = double>
class vector2;

template<typename T = double>
class vector3;

template<typename T = double>
class vector4;

template<typename T = double>
class matrix33;

template<typename T = double>
class matrix44;

typedef vector2<double> vector2d;
typedef vector2<float>  vector2f;
typedef vector3<double> vector3d;
typedef vector3<float>  vector3f;
typedef vector4<double> vector4d;
typedef vector4<float>  vector4f;

}
#endif // MATRICES_H
