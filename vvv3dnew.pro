TEMPLATE = app
CONFIG += console c++14
CONFIG -= qt

DESTDIR = bin
OBJECTS_DIR = tmp
QMAKE_INCDIR += src
QMAKE_INCDIR += /usr/include/freetype2


DEFINES += GLEW_STATIC
QMAKE_CXXFLAGS += -Wall -Wextra
LIBS += -lGLEW -lGL -lglut -lpng -lfreetype
#LIBS += -lGLESv2
#LIBS += -l3ds

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

HEADERS += \
    src/core/engine.h \
    src/core/transform.h \
    src/core/graphics/fonts/font.h \
    src/core/graphics/fonts/mgrfreetype.h \
    src/core/graphics/fonts/textline.h \
    src/core/graphics/lowlevel/lowlevelbuffer.h \
    src/core/graphics/lowlevel/lowlevelgeometry.h \
    src/core/graphics/lowlevel/lowlevelshader.h \
    src/core/graphics/lowlevel/lowlevelshaderprogram.h \
    src/core/graphics/lowlevel/lowleveltexture.h \
    src/core/graphics/lowlevel/openglprovider.h \
    src/core/graphics/lowlevel/vertexattribute.h \
    src/core/graphics/textures/texture.h \
    src/core/graphics/textures/texturepacker.h \
    src/core/graphics/textures/textureatlas.h \
    src/core/graphics/textures/texturemanager.h \
    src/core/graphics/shaders/shader.h \
    src/core/graphics/camera.h \
    src/core/graphics/colour.h \
    src/core/graphics/framebufferobject.h \
    src/core/graphics/geometry.h \
    src/utils/helper.h \
    src/utils/myutils.h \
    src/vvvmath/linalg.h \
    src/vvvmath/mathconstants.h \
    src/vvvmath/matrices_types.h \
    src/vvvmath/matrix33.h \
    src/vvvmath/matrix44.h \
    src/vvvmath/polynomial.h \
    src/vvvmath/quaternion.h \
    src/vvvmath/vector2.h \
    src/vvvmath/vector3.h \
    src/vvvmath/vector4.h \
    src/std/draw.h \
    src/core/resourcemanager.h

SOURCES += \
    src/core/engine.cpp \
    src/core/graphics/fonts/font.cpp \
    src/core/graphics/fonts/textline.cpp \
    src/core/graphics/lowlevel/lowlevelbuffer.cpp \
    src/core/graphics/lowlevel/lowlevelgeometry.cpp \
    src/core/graphics/lowlevel/lowlevelshader.cpp \
    src/core/graphics/lowlevel/lowlevelshaderprogram.cpp \
    src/core/graphics/lowlevel/lowleveltexture.cpp \
    src/core/graphics/lowlevel/vertexattribute.cpp \
    src/core/graphics/textures/texture.cpp \
    src/core/graphics/textures/texturepacker.cpp \
    src/core/graphics/textures/textureatlas.cpp \
    src/core/graphics/textures/texturemanager.cpp \
    src/core/graphics/shaders/shader.cpp \
    src/core/graphics/camera.cpp \
    src/core/graphics/colour.cpp \
    src/core/graphics/framebufferobject.cpp \
    src/core/graphics/geometry.cpp \
    src/utils/helper.cpp \
    src/utils/myutils.cpp \
    src/main.cpp \
    src/std/draw.cpp

