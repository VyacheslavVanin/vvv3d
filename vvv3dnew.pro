TEMPLATE = app
CONFIG += console
CONFIG -= qt

DESTDIR = bin
OBJECTS_DIR = tmp
QMAKE_INCDIR += src
QMAKE_INCDIR += /usr/include/freetype2


DEFINES += GLEW_STATIC
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra
LIBS += -lGLEW -lGL -lglut -lpng -lfreetype
LIBS += -lGLESv2
#LIBS += -l3ds

DISTFILES += \
    resources/shaders/basic.fsh \
    resources/shaders/sprite.fsh \
    resources/shaders/rectanglewidget.fsh \
    resources/shaders/basic.vsh \
    resources/shaders/sprite.vsh \
    resources/shaders/rectanglewidget.vsh \
    resources/shaders/text.fsh \
    resources/shaders/text.vsh
shaders.path = bin/data/shaders
shaders.files = $$DISTFILES
INSTALLS += shaders

HEADERS += \
    src/core/engine.h \
    src/core/transform.h \
    src/graphics/fonts/font.h \
    src/graphics/fonts/mgrfreetype.h \
    src/graphics/fonts/textline.h \
    src/graphics/lowlevel/lowlevelbuffer.h \
    src/graphics/lowlevel/lowlevelgeometry.h \
    src/graphics/lowlevel/lowlevelshader.h \
    src/graphics/lowlevel/lowlevelshaderprogram.h \
    src/graphics/lowlevel/lowleveltexture.h \
    src/graphics/lowlevel/openglprovider.h \
    src/graphics/lowlevel/vertexattribute.h \
    src/graphics/textures/texture.h \
    src/graphics/textures/texturepacker.h \
    src/graphics/camera.h \
    src/graphics/colour.h \
    src/graphics/framebufferobject.h \
    src/graphics/geometry.h \
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
    src/graphics/shaders/shader.h \
    src/graphics/iobject.h \
    src/core/ILayer.h \
    src/core/layeredengine.h \
    src/std/spritelayer.h \
    src/std/sprite.h \
    src/graphics/textures/textureatlas.h \
    src/graphics/textures/texturemanager.h

SOURCES += \
    src/core/engine.cpp \
    src/graphics/fonts/font.cpp \
    src/graphics/fonts/textline.cpp \
    src/graphics/lowlevel/lowlevelbuffer.cpp \
    src/graphics/lowlevel/lowlevelgeometry.cpp \
    src/graphics/lowlevel/lowlevelshader.cpp \
    src/graphics/lowlevel/lowlevelshaderprogram.cpp \
    src/graphics/lowlevel/lowleveltexture.cpp \
    src/graphics/lowlevel/vertexattribute.cpp \
    src/graphics/textures/texture.cpp \
    src/graphics/textures/texturepacker.cpp \
    src/graphics/camera.cpp \
    src/graphics/colour.cpp \
    src/graphics/framebufferobject.cpp \
    src/graphics/geometry.cpp \
    src/utils/helper.cpp \
    src/utils/myutils.cpp \
    src/main.cpp \
    src/graphics/shaders/shader.cpp \
    src/graphics/iobject.cpp \
    src/core/layeredengine.cpp \
    src/std/spritelayer.cpp \
    src/std/sprite.cpp \
    src/graphics/textures/textureatlas.cpp \
    src/graphics/textures/texturemanager.cpp

