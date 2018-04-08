#include "engine.hpp"
#include "utils/helper.hpp"
#include <chrono>
#include <core/graphics/lowlevel/openglprovider.hpp>
#include <core/resourcemanager.hpp>
#include <core/sdllayer.hpp>

namespace vvv3d {

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
      resourceManager(), input(), gui_layer()
{
    bench timings("Engine base initialization");
    bench("init hal context"), hal->initContext(argc, argv);
    bench("create window"),
        hal->createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
                          windowName);
    hal->setDisplayFunction([]() { activeEngine->display(); });
    hal->setIdleFunction([]() {});
    hal->setResizeFunction([](int x, int y) { activeEngine->resize(x, y); });

    bench("init resource manager"),
        resourceManager.reset(new ResourceManager());

    glClearColor(0.05f, 0.1f, 0.2f, 0);
    glEnable(GL_DEPTH_TEST);

    activeEngine = this;
    resourceManager->getAnimationManager().init();
    gui_layer.setVisible(true);
}

Engine::~Engine() {}

void Engine::run()
{
    bench("initial setup"), initialSetup();
    const auto width = getViewportWidth();
    const auto height = getViewportHeight();
    onResize(width, height);
    gui().resize(width, height);
    hal->mainLoop();
}

void Engine::stop() { hal->stopMainLoop(); }

ResourceManager& Engine::getResourceManager() { return *resourceManager; }

const Input& Engine::getInput() const { return input; }

void Engine::display()
{
    clock.updateFrameTime();

    const auto t1 = std::chrono::system_clock::now();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    onDraw();
    gui().draw();
    gui().processInputEvents(getInput());

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
    gui().resize(x, y);
    onResize(viewportWidth, viewportHeight);
}

float Engine::getCurrentFps() const { return currentfps; }

void Engine::setVSync(bool vsync) { hal->setVSync(vsync); }
GuiLayer& Engine::gui() { return gui_layer; }

double Engine::time() { return clock.currentFrameTime(); }

double Engine::getTimeSinceLastFrame() { return clock.sinceLastFrame(); }

TextureManager& getTextureManager()
{
    return Engine::getActiveEngine().getResourceManager().getTextureManager();
}

FontManager& getFontManager()
{
    return Engine::getActiveEngine().getResourceManager().getFontManager();
}

GeometryManager& getGeometryManager()
{
    return Engine::getActiveEngine().getResourceManager().getGeometryManager();
}

ShaderManager& getShaderManager()
{
    return Engine::getActiveEngine().getResourceManager().getShaderManager();
}

AnimationManager& getAnimationManager()
{
    return Engine::getActiveEngine().getResourceManager().getAnimationManager();
}

void Engine::load(const vvv::CfgNode& node) {
    getTextureManager().load(node);
    getAnimationManager().load(node);
    gui().load(node);
}

void Engine::load(std::istream& stream)
{
    const auto& cfg = vvv::make_cfg(stream);
    load(cfg);
}

void Engine::load(const std::string& string)
{
    std::stringstream ss;
    ss << string;
    load(ss);
}

} // namespace vvv3d
