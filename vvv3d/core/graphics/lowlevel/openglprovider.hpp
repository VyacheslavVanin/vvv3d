#pragma once

#ifdef VVV3D_BACKEND_QT
#include <vvv3d/private/qt_hal_impl/qtopenglprovider.hpp>
#else
#ifdef VVV3D_USE_OPENGL_CORE
#include <GL/gl.h>
#include <GL/glew.h>
#else
#include <GLES3/gl32.h>
#include <stdlib.h>
#endif
#endif
