#include "engine.h"
#include <iostream>
#include <chrono>
#include <core/graphics/lowlevel/openglprovider.h>
#include <core/camera.h>
#include <core/resourcemanager.h>
#include <core/sdllayer.h>

static const int DEFAULT_SCREEN_WIDTH = 640;
static const int DEFAULT_SCREEN_HEIGHT = 480;
Engine* Engine::activeEngine = nullptr;

Engine::Engine(int argc, char** argv, const char* windowName)
    : currentfps(0), viewportWidth(DEFAULT_SCREEN_WIDTH),
      viewportHeight(DEFAULT_SCREEN_HEIGHT),
      resourceManager(new ResourceManager()),
      hal(new sdlLayer(argc, argv, GLPROFILE::CORE, 3, 3))
{
    hal->initContext(argc, argv);
    hal->createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, windowName);
    hal->setDisplayFunction([](){activeEngine->display();});
    hal->setIdleFunction([](){});
    hal->setResizeFunction([](int x, int y){activeEngine->resize(x,y);});

    glClearColor( 0.05f, 0.1f, 0.2f, 0);
    glEnable( GL_DEPTH_TEST );

    activeEngine = this;
}

Engine::~Engine() {}

void Engine::run()
{
    initialSetup();
    hal->mainLoop();
}

ResourceManager& Engine::getResourceManager() {return *resourceManager;}

const Input& Engine::getInput() const
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

const Input::Keyboard& Input::getKeyboard() const
{
    return keyboard;
}

const Input::Mouse&Input::getMouse() const
{
    return mouse;
}

bool Input::Keyboard::keyDown(uint16_t scancode) const
{
    return Engine::getActiveEngine().hal->keyDown(scancode);
}

bool Input::Keyboard::hasText() const
{
    return Engine::getActiveEngine().hal->hasText();
}

const std::string& Input::Keyboard::getText() const
{
    return Engine::getActiveEngine().hal->getText();
}

bool Input::Mouse::buttonDown(uint16_t button) const
{
    return Engine::getActiveEngine().hal->mouseButtonDown(button);
}

vvv::vector2i Input::Mouse::getMousePos() const
{
    return vvv::vector2i(getMouseX(), getMouseY());
}

vvv::vector2i Input::Mouse::getMouseRel() const
{
    return vvv::vector2i(getMouseRelX(), getMouseRelY());
}

int Input::Mouse::getMouseX() const
{
    return Engine::getActiveEngine().hal->getMouseX();
}

int Input::Mouse::getMouseY() const
{
    return Engine::getActiveEngine().hal->getMouseY();
}

int Input::Mouse::getMouseRelX() const
{
    return Engine::getActiveEngine().hal->getMouseRelX();
}

int Input::Mouse::getMouseRelY() const
{
    return Engine::getActiveEngine().hal->getMouseRelY();
}

bool Input::Mouse::isMoved() const
{
    return Engine::getActiveEngine().hal->isMouseMoved();
}
