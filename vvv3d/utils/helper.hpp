#pragma once
#include <chrono>
#include <vvv3d/core/graphics/geometry.hpp>

namespace vvv3d {
class bench {
public:
    bench(const std::string& msg);
    ~bench();

    static void setEnable(bool value) { enable = value; }
private:
    std::string msg;
    std::chrono::system_clock::time_point start;
    static thread_local int depth;
    static int enable;
};

Geometry* loadGeometryFrom_vvv3d(const char* filename);

#if defined ENABLE_3DS
Geometry* loadGeometryFrom3ds(const char* filename);
#endif
} // namespace vvv3d
