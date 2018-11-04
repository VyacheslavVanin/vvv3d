#include "engine.hpp"
#include "utils/helper.hpp"
#include <chrono>
#include <core/graphics/lowlevel/openglprovider.hpp>
#include <core/resourcemanager.hpp>
#include <core/sdllayer.hpp>

namespace vvv3d {

namespace {
void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                     GLsizei length, const GLchar* message,
                     const void* userParam)
{
    fprintf(stderr,
            "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
            severity, message);
}
} // namespace

static const int DEFAULT_SCREEN_WIDTH = 640;
static const int DEFAULT_SCREEN_HEIGHT = 480;
Engine* Engine::activeEngine = nullptr;
Time<> Engine::clock;

Engine::Engine(int argc, char** argv, const char* windowName)
    : currentfps(0), viewport(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT),
#ifdef VVV3D_USE_OPENGL_CORE
      hal(new sdlLayer(argc, argv, GLPROFILE::CORE, 3, 3)),
#else
      hal(new sdlLayer(argc, argv, GLPROFILE::ES, 3, 0)),
#endif
      resourceManager(), input(), gui_layer(), clear_color(0.05f, 0.1f, 0.2f, 0)
{
    bench timings("Engine base initialization");
    bench("init hal context"), hal->initContext(argc, argv);
    bench("create window"),
        hal->createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
                          windowName);
#ifdef DEBUG_GL
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
#endif
    hal->setDisplayFunction([]() { activeEngine->display(); });
    hal->setIdleFunction([]() {});
    hal->setResizeFunction([](int x, int y) { activeEngine->resize(x, y); });

    bench("init resource manager"),
        resourceManager.reset(new ResourceManager());

    setClearColor(clear_color);
    glEnable(GL_DEPTH_TEST);

    activeEngine = this;
    resourceManager->getAnimationManager().init();
    gui_layer.setVisible(true);
}

Engine::~Engine() {}

void Engine::run()
{
    bench("initial setup"), initialSetup();
    const auto width = getViewport().getWidth();
    const auto height = getViewport().getHeight();
    onResize(width, height);
    gui().resize(width, height);
    is_running.store(true);
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

const Viewport& Engine::getViewport() const { return viewport; }

Engine& Engine::getActiveEngine() { return *activeEngine; }

void Engine::resize(int x, int y)
{
    y = (y != 0) ? y : 1;
    viewport.set(x, y);
    glViewport(0, 0, x, y);
    gui().resize(x, y);
    onResize(x, y);
}

float Engine::getCurrentFps() const { return currentfps; }

void Engine::setVSync(bool vsync) { hal->setVSync(vsync); }
bool Engine::isRunning() const { return is_running.load(); }

GuiLayer& Engine::gui() { return gui_layer; }

double Engine::frameTime() { return clock.currentFrameTime(); }
double Engine::currentTime() { return clock.sinceEpoch(); }

int64_t Engine::currentTimeMillis()
{
    return clock.sinceEpoch<std::chrono::milliseconds>();
}

int64_t Engine::currentTimeMicros()
{
    return clock.sinceEpoch<std::chrono::microseconds>();
}

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

void Engine::load(const vvv::CfgNode& node)
{
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

void Engine::setClearColor(const vvv3d::Color& color) {
    clear_color = color;
    glClearColor(color.r, color.g, color.b, color.a);
}
const vvv3d::Color& Engine::getClearColor() const { return clear_color; }

} // namespace vvv3d
