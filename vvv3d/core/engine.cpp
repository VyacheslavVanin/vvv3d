#include "engine.hpp"
#include <chrono>
#include <core/graphics/lowlevel/openglprovider.hpp>
#include <core/resourcemanager.hpp>
#include <core/sdllayer.hpp>

using namespace vvv3d;

static const int DEFAULT_SCREEN_WIDTH = 640;
static const int DEFAULT_SCREEN_HEIGHT = 480;
Engine* Engine::activeEngine = nullptr;
Time<> Engine::clock;

Engine::Engine(int argc, char** argv, const char* windowName)
    : currentfps(0), viewportWidth(DEFAULT_SCREEN_WIDTH),
      viewportHeight(DEFAULT_SCREEN_HEIGHT),
#ifdef VVV3D_USE_OPENGL_CORE
      hal(new sdlLayer(argc, argv, GLPROFILE::CORE, 3, 3)),
#else
      hal(new sdlLayer(argc, argv, GLPROFILE::ES, 3, 2)),
#endif
      resourceManager(), input()
{
    hal->initContext(argc, argv);
    hal->createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, windowName);
    hal->setDisplayFunction([]() { activeEngine->display(); });
    hal->setIdleFunction([]() {});
    hal->setResizeFunction([](int x, int y) { activeEngine->resize(x, y); });

    resourceManager.reset(new ResourceManager());

    glClearColor(0.05f, 0.1f, 0.2f, 0);
    glEnable(GL_DEPTH_TEST);

    activeEngine = this;
}

Engine::~Engine() {}

void Engine::run()
{
    initialSetup();
    onResize(getViewportWidth(), getViewportHeight());
    hal->mainLoop();
}

ResourceManager& Engine::getResourceManager() { return *resourceManager; }

const Input& Engine::getInput() const { return input; }

void Engine::display()
{
    clock.updateFrameTime();

    const auto t1 = std::chrono::system_clock::now();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    onDraw();

    hal->swap();
    const auto t2 = std::chrono::system_clock::now();
    const auto dt = t2 - t1;
    const auto milis =
        (float)(std::chrono::duration_cast<std::chrono::microseconds>(dt)
                    .count()) /
        1000;
    currentfps = 1000.0f / milis;
}

void Engine::initialSetup() {}
void Engine::onDraw() {}
void Engine::onResize(int x, int y) { (void)x, (void)y; }

int Engine::getViewportWidth() const { return viewportWidth; }
int Engine::getViewportHeight() const { return viewportHeight; }

Engine& Engine::getActiveEngine() { return *activeEngine; }

void Engine::resize(int x, int y)
{
    y = (y != 0) ? y : 1;
    viewportHeight = y;
    viewportWidth = x;
    glViewport(0, 0, viewportWidth, viewportHeight);
    onResize(viewportWidth, viewportHeight);
}

float Engine::getCurrentFps() const { return currentfps; }

void Engine::setVSync(bool vsync) { hal->setVSync(vsync); }

double Engine::time() { return clock.currentFrameTime(); }

double Engine::getTimeSinceLastFrame() { return clock.sinceLastFrame(); }
