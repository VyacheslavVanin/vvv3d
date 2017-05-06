#ifndef MYUTILS_H
#define MYUTILS_H
#include <iostream>
#include <string>
#include <vvv3d/core/graphics/lowlevel/openglprovider.h>

#define DebugVar(var)                                                          \
    {                                                                          \
        std::cout << #var << " = " << var << std::endl;                        \
    }
#define GLSL(version, src) "#version " #version "\n" #src

namespace vvv3d {
void loadFileToString(const char* filename, std::string& out);
std::string loadFileToString(const char* filename);
}

#endif // MYUTILS_H
