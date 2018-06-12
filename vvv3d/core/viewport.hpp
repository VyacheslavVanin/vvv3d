#pragma once
#include <vvv3d/vvvmath/linalg.hpp>

namespace vvv3d {
class Viewport {
public:
    Viewport() : size() {}
    Viewport(const vvv::vector2i& v) : size(v) {}
    Viewport(int width, int height) : size(width, height) {}

    int getWidth() const { return size.x; }
    int getHeight() const { return size.y; };
    const vvv::vector2i& getSize() const { return size; }
    float getAspect() const { return getWidth() / float(getHeight()); }

    void set(int width, int height) { size.set(width, height); }
    void set(const vvv::vector2i& v) { size = v; }
    void setHeight(int height) { size.y = height; }
    void setWidth(int width) { size.x = width; }

private:
    vvv::vector2i size;
};
} // namespace vvv3d
