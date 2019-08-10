#pragma once
#include <vvv3d/core/graphics/color.hpp>
#include <vvv3d/vvvmath/linalg.hpp>

namespace vvv3d {
class Light {
public:
    enum class Type { POINT, DIRECTIONAL, SPOT, AMBIENT };

    Light(Type type) : pos(), dir(), color(), type(type), intensity(100) {}

    static Light makeDirectional(const vvv3d::Color& color,
                                 const vvv::vector3f& direction,
                                 float intensity);
    static Light makeAmbient(const vvv3d::Color& color, float intensity);
    static Light makePoint(const vvv3d::Color& color,
                           const vvv::vector3f& position, float intensity);
    static Light makeSpot(const vvv3d::Color& color,
                          const vvv::vector3f& position,
                          const vvv::vector3f& direction, float intensity,
                          float cutoff, float exponent);

    const vvv::vector3f& getPosition() const { return pos; }
    const vvv::vector3f& getDirection() const { return dir; }
    const vvv3d::Color& getColor() const { return color; }
    Type getType() const { return type; }
    float getIntensity() const { return intensity; }
    float getCutoff() const { return cutoff; }
    float getExponent() const { return exponent; }
    bool isCastShadow() const { return can_cast_shadow; }

    void setPosition(const vvv::vector3f& position) { pos = position; }
    void setDirection(const vvv::vector3f& direction)
    {
        dir = direction.normalized();
    }
    void setColor(const vvv3d::Color& color) { this->color = color; }
    void setIntensity(float intensity) { this->intensity = intensity; }
    void setCutoff(float cutoff) { this->cutoff = cutoff; }
    void setExponent(float exponent) { this->exponent = exponent; }
    void setShadow(bool shadow) { this->can_cast_shadow = shadow; }

private:
    vvv::vector3f pos;
    vvv::vector3f dir;
    vvv3d::Color color;
    Type type;
    float intensity;
    float cutoff = 0;
    float exponent = 0;
    bool can_cast_shadow = true;
};
} // namespace vvv3d
