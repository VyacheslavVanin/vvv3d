TEMPLATE = lib
CONFIG += console c++14
CONFIG -= qt

DESTDIR = build
OBJECTS_DIR = tmp
QMAKE_INCDIR += vvv3d
QMAKE_INCDIR += /usr/include/freetype2


DEFINES += GLEW_STATIC
QMAKE_CXXFLAGS += -Wall -Wextra
LIBS += -lGLEW -lGL -lglut -lpng -lfreetype
#LIBS += -lGLESv2
#LIBS += -l3ds

HEADERS += \
    vvv3d/core/engine.h \
    vvv3d/core/transform.h \
    vvv3d/core/camera.h \
    vvv3d/core/graphics/fonts/font.h \
    vvv3d/core/graphics/fonts/mgrfreetype.h \
    vvv3d/core/graphics/fonts/textline.h \
    vvv3d/core/graphics/lowlevel/lowlevelbuffer.h \
    vvv3d/core/graphics/lowlevel/lowlevelgeometry.h \
    vvv3d/core/graphics/lowlevel/lowlevelshader.h \
    vvv3d/core/graphics/lowlevel/lowlevelshaderprogram.h \
    vvv3d/core/graphics/lowlevel/lowleveltexture.h \
    vvv3d/core/graphics/lowlevel/openglprovider.h \
    vvv3d/core/graphics/lowlevel/vertexattribute.h \
    vvv3d/core/graphics/textures/texture.h \
    vvv3d/core/graphics/textures/texturepacker.h \
    vvv3d/core/graphics/textures/textureatlas.h \
    vvv3d/core/graphics/textures/texturemanager.h \
    vvv3d/core/graphics/shaders/shader.h \
    vvv3d/core/graphics/colour.h \
    vvv3d/core/graphics/framebufferobject.h \
    vvv3d/core/graphics/geometry.h \
    vvv3d/utils/helper.h \
    vvv3d/utils/myutils.h \
    vvv3d/vvvmath/linalg.h \
    vvv3d/vvvmath/mathconstants.h \
    vvv3d/vvvmath/matrices_types.h \
    vvv3d/vvvmath/matrix33.h \
    vvv3d/vvvmath/matrix44.h \
    vvv3d/vvvmath/polynomial.h \
    vvv3d/vvvmath/quaternion.h \
    vvv3d/vvvmath/vector2.h \
    vvv3d/vvvmath/vector3.h \
    vvv3d/vvvmath/vector4.h \
    vvv3d/std/draw.h \
    vvv3d/core/resourcemanager.h \
    vvv3d/vvv3d.h

SOURCES += \
    vvv3d/core/engine.cpp \
    vvv3d/core/camera.cpp \
    vvv3d/core/graphics/fonts/font.cpp \
    vvv3d/core/graphics/fonts/textline.cpp \
    vvv3d/core/graphics/lowlevel/lowlevelbuffer.cpp \
    vvv3d/core/graphics/lowlevel/lowlevelgeometry.cpp \
    vvv3d/core/graphics/lowlevel/lowlevelshader.cpp \
    vvv3d/core/graphics/lowlevel/lowlevelshaderprogram.cpp \
    vvv3d/core/graphics/lowlevel/lowleveltexture.cpp \
    vvv3d/core/graphics/lowlevel/vertexattribute.cpp \
    vvv3d/core/graphics/textures/texture.cpp \
    vvv3d/core/graphics/textures/texturepacker.cpp \
    vvv3d/core/graphics/textures/textureatlas.cpp \
    vvv3d/core/graphics/textures/texturemanager.cpp \
    vvv3d/core/graphics/shaders/shader.cpp \
    vvv3d/core/graphics/colour.cpp \
    vvv3d/core/graphics/framebufferobject.cpp \
    vvv3d/core/graphics/geometry.cpp \
    vvv3d/utils/helper.cpp \
    vvv3d/utils/myutils.cpp \
    vvv3d/std/draw.cpp

INSTALL_HEADERS = $$HEADERS
INSTALL_PREFIX  = $$DESTDIR
for(header, INSTALL_HEADERS) {
  path = $${INSTALL_PREFIX}/$${dirname(header)}
  eval(headers_$${path}.files += $$header)
  eval(headers_$${path}.path = $$path)
  eval(INSTALLS *= headers_$${path})
}

DISTFILES += \
    resources/shaders/basic.fsh \
    resources/shaders/sprite.fsh \
    resources/shaders/basic.vsh \
    resources/shaders/sprite.vsh \
    resources/shaders/text.fsh \
    resources/shaders/text.vsh
shaders.path = bin/data/shaders
shaders.files = $$DISTFILES

INSTALLS += shaders
