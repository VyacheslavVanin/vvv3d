TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_INCDIR += ../vvv3dnew/build/
QMAKE_LIBDIR += ../vvv3dnew/build/

QMAKE_CXXFLAGS += -Wall -Wextra
LIBS += -lvvv3dnew -lGLEW -lGL -lglut -lpng -lfreetype


SOURCES += main.cpp \
    gui/guiobject.cpp


DISTFILES += \
    resources/shaders/basic.fsh \
    resources/shaders/sprite.fsh \
    resources/shaders/basic.vsh \
    resources/shaders/sprite.vsh \
    resources/shaders/text.fsh \
    resources/shaders/text.vsh \
    resources/shaders/solidcolor.fsh \
    resources/shaders/solidcolor.vsh
shaders.path = data/shaders
shaders.files = $$DISTFILES

INSTALLS += shaders

HEADERS += \
    gui/guiobject.h
