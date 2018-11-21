#include "light.hpp"

namespace vvv3d {

Light Light::makeDirectional(const vvv3d::Color& color,
                             const vvv::vector3f& direction, float intensity)
{
    Light ret(Type::DIRECTIONAL);
    ret.color = color;
    ret.dir = direction;
    ret.intensity = intensity;

    return ret;
}

Light Light::makeAmbient(const vvv3d::Color& color, float intensity)
{
    Light ret(Type::AMBIENT);
    ret.color = color;
    ret.intensity = intensity;

    return ret;
}

Light Light::makePoint(const vvv3d::Color& color, const vvv::vector3f& position,
                       float intensity)
{
    Light ret(Type::POINT);
    ret.color = color;
    ret.pos = position;
    ret.intensity = intensity;

    return ret;
}

Light Light::makeSpot(const vvv3d::Color& color, const vvv::vector3f& position,
                      const vvv::vector3f& direction, float intensity,
                      float cutoff, float exponent)
{
    Light ret(Type::SPOT);
    ret.color = color;
    ret.pos = position;
    ret.dir = direction;
    ret.intensity = intensity;
    ret.cutoff = cutoff;
    ret.exponent = exponent;

    return ret;
}

} // namespace vvv3d
