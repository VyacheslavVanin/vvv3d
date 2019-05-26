#pragma once
#include <sstream>
#include <vvv3d/core/graphics/lowlevel/openglprovider.hpp>
#include <vvv3d/std/log.hpp>

#define DebugVar(var)                                                          \
    {                                                                          \
        std::stringstream ss;                                                  \
        ss << #var << " = " << var << std::endl;                               \
        LOG(ss.str());                                                         \
    }
#define GLSL(version, src) "#version " #version "\n" #src

namespace vvv3d {
void loadFileToString(const char* filename, std::string& out);
std::string loadFileToString(const char* filename);
} // namespace vvv3d
