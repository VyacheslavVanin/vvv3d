#include "engine.h"
#include <iostream>
#include <chrono>
#include <core/graphics/lowlevel/openglprovider.h>
#include <core/camera.h>
#include <core/resourcemanager.h>
#include <core/gluthal.h>
#include <core/sdllayer.h>

static const int DEFAULT_SCREEN_WIDTH = 640;
static const int DEFAULT_SCREEN_HEIGHT = 480;
Engine* Engine::activeEngine = nullptr;

Engine::Engine(int argc, char** argv, const char* windowName)
    : currentfps(0), viewportWidth(DEFAULT_SCREEN_WIDTH),
      viewportHeight(DEFAULT_SCREEN_HEIGHT),
      resourceManager(new ResourceManager()),
      hal(new sdlLayer(argc, argv))
{
    hal->initContext(argc, argv);
    hal->createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, windowName);
    hal->setDisplayFunction([](){activeEngine->display();});
    hal->setIdleFunction([](){});
    hal->setResizeFunction([](int x, int y){activeEngine->resize(x,y);});

    glClearColor( 0.05f, 0.1f, 0.2f, 0);
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_MULTISAMPLE);

    activeEngine = this;
}

Engine::~Engine() {}

void Engine::run()
{
    initialSetup();
    hal->mainLoop();
}

ResourceManager& Engine::getResourceManager() {return *resourceManager;}

const Engine::Input&Engine::getInput() const
{
    return input;
}

void Engine::display()
{
    const auto t1 = std::chrono::system_clock::now();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    onDraw();

    hal->swap();
    const auto t2 = std::chrono::system_clock::now();
    const auto dt = t2 -t1;
    const auto milis = (float)(std::chrono::duration_cast<std::chrono::microseconds>(dt).count())/1000;
    currentfps = 1000.0f / milis;
}

void Engine::initialSetup() {}
void Engine::onDraw() {}
void Engine::onResize(int x, int y) {(void)x, (void)y;}


int Engine::getVieportWidth() const {return viewportWidth;}
int Engine::getVieportHeight() const {return viewportHeight;}

Engine& Engine::getActiveEngine()
{
    return *activeEngine;
}

void Engine::resize(int x, int y)
{
    y = (y!=0) ? y : 1;
    viewportHeight = y;
    viewportWidth  = x;
    glViewport( 0, 0, viewportWidth, viewportHeight);
    onResize(viewportWidth, viewportHeight);
}

float Engine::getCurrentFps() const {return currentfps;}

const Engine::Input::Keyboard& Engine::Input::getKeyboard() const
{
    return keyboard;
}

const Engine::Input::Mouse&Engine::Input::getMouse() const
{
    return mouse;
}

bool Engine::Input::Keyboard::keyDown(uint16_t scancode) const
{
    return Engine::getActiveEngine().hal->keyDown(scancode);
}

bool Engine::Input::Keyboard::hasText() const
{
    return Engine::getActiveEngine().hal->hasText();
}

const std::string& Engine::Input::Keyboard::getText() const
{
    return Engine::getActiveEngine().hal->getText();
}

bool Engine::Input::Mouse::buttonDown(uint16_t button) const
{
    return Engine::getActiveEngine().hal->mouseButtonDown(button);
}

vvv::vector2i Engine::Input::Mouse::getMousePos() const
{
    return vvv::vector2i(getMouseX(), getMouseY());
}

vvv::vector2i Engine::Input::Mouse::getMouseRel() const
{
    return vvv::vector2i(getMouseRelX(), getMouseRelY());
}

int Engine::Input::Mouse::getMouseX() const
{
    return Engine::getActiveEngine().hal->getMouseX();
}

int Engine::Input::Mouse::getMouseY() const
{
    return Engine::getActiveEngine().hal->getMouseY();
}

int Engine::Input::Mouse::getMouseRelX() const
{
    return Engine::getActiveEngine().hal->getMouseRelX();
}

int Engine::Input::Mouse::getMouseRelY() const
{
    return Engine::getActiveEngine().hal->getMouseRelY();
}
