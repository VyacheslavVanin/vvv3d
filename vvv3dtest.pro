TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_INCDIR += ../vvv3dnew/build/
QMAKE_LIBDIR += ../vvv3dnew/build/

QMAKE_CXXFLAGS += -Wall -Wextra
LIBS += -lvvv3dnew -lGLEW -lGL -lglut


SOURCES += main.cpp \
    gui/guilayer.cpp \
    gui/imagewidget.cpp \
    gui/colorrectanglewidget.cpp \
    gui/widget.cpp \
    gui/textwidget.cpp \
    gui/verticallayout.cpp \
    gui/layout.cpp \
    gui/panel.cpp


DISTFILES += \
    resources/shaders/basic.fsh \
    resources/shaders/sprite.fsh \
    resources/shaders/basic.vsh \
    resources/shaders/sprite.vsh \
    resources/shaders/text.fsh \
    resources/shaders/text.vsh \
    resources/shaders/imagewidget.fsh \
    resources/shaders/imagewidget.vsh \
    resources/shaders/solidrect.fsh \
    resources/shaders/solidrect.vsh
shaders.path = data/shaders
shaders.files = $$DISTFILES

INSTALLS += shaders

HEADERS += \
    gui/guilayer.h \
    gui/imagewidget.h \
    gui/colorrectanglewidget.h \
    gui/widget.h \
    gui/textwidget.h \
    gui/verticallayout.h \
    gui/rect.h \
    gui/layout.h \
    gui/align.h \
    gui/panel.h
