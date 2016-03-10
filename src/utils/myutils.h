#ifndef MYUTILS_H
#define MYUTILS_H
#include <iostream>
#include <string>
#include <GL/gl.h>

#define DebugVar(var) {std::cout << #var << " = " << var << std::endl;}
#define GLSL(version, src) "#version " #version "\n" #src

void loadFileToString(const char* filename, std::string& out);
std::string loadFileToString(const char* filename);

GLsizei     sizeOfComponent(GLenum componentType);



#endif // MYUTILS_H
