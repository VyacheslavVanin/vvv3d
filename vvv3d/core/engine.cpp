#include "engine.hpp"
#include "utils/helper.hpp"
#include <chrono>
#include <core/graphics/lowlevel/openglprovider.hpp>
#include <core/resourcemanager.hpp>
#include <sstream>
#ifdef VVV3D_BACKEND_SDL
#include <vvv3d/private/sdl_hal_impl/sdllayer.hpp>
#elif VVV3D_BACKEND_QT
#include <vvv3d/private/qt_hal_impl/qthallayer.hpp>
#endif
#include <vvv3d/std/log.hpp>

namespace vvv3d {

namespace {

void enableOpenGLDebug()
{
#ifdef DEBUG_GL
    auto MessageCallback = [](GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar* message, const void* userParam) {
        using vvv::helper::format;
        std::ignore = source;
        std::ignore = length;
        std::ignore = userParam;
        std::ignore = id;
        LOG(format("GL CALLBACK: @ type = @, severity = @, message = @",
                   (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
                   severity, message));
        if (type == GL_DEBUG_TYPE_ERROR)
            abort();
    };
    LOG("Debug output enabled...");
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
#endif
}

GLint getGLInteger(GLenum pname)
{
    GLint ret;
    glGetIntegerv(pname, &ret);
    return ret;
}

void PrintGLParams()
{
    using vvv::helper::format;
    const auto max_vertex_attribs = getGLInteger(GL_MAX_VERTEX_ATTRIBS);
    const auto max_texture_size = getGLInteger(GL_MAX_TEXTURE_SIZE);
    const auto max_texture_units = getGLInteger(GL_MAX_TEXTURE_IMAGE_UNITS);
    const auto max_uniform_locations = getGLInteger(GL_MAX_UNIFORM_LOCATIONS);
    const auto max_color_attachments = getGLInteger(GL_MAX_COLOR_ATTACHMENTS);
    LOG(format("GL_MAX_TEXTURE_SIZE = @", max_texture_size));
    LOG(format("GL_MAX_TEXTURE_IMAGE_UNITS = @", max_texture_units));
    LOG(format("GL_MAX_UNIFORM_LOCATIONS = @", max_uniform_locations));
    LOG(format("GL_MAX_COLOR_ATTACHMENTS = @", max_color_attachments));
    LOG(format("GL_MAX_VERTEX_ATTRIBS = @", max_vertex_attribs));

    auto& e = vvv3d::Engine::getActiveEngine();
    const auto& display_properties = e.getHAL().GetDisplayProperies();
    const auto& resolution = display_properties.resolution;
    LOG(format("Display resolution = @x@", resolution.x, resolution.y));
    LOG(format("Display dpi = @", display_properties.dpi.x));
}

void enableBlend(bool blend)
{
    if (blend) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {
        glDisable(GL_BLEND);
    }
}

} // namespace

static const int DEFAULT_SCREEN_WIDTH = 640;
static const int DEFAULT_SCREEN_HEIGHT = 480;
Engine* Engine::activeEngine = nullptr;
Time<> Engine::clock;

Engine::Engine(int argc, char** argv, const char* windowName)
    : currentfps(0), viewport(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT),
#ifdef VVV3D_BACKEND_SDL
#ifdef VVV3D_USE_OPENGL_CORE
      hal(new sdlLayer(argc, argv, GLPROFILE::CORE, 3, 3)),
#else
      hal(new sdlLayer(argc, argv, GLPROFILE::ES, 3, 0)),
#endif
#elif VVV3D_BACKEND_QT
      hal(new QtHalLayer()),
#endif
      resourceManager(), input(), gui_layer(), clear_color(vvv3d::BLACK)
{
    activeEngine = this;
    bench timings("Engine base initialization");
    bench("init hal context"), hal->initContext(argc, argv);
    bench("create window"),
        hal->createWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
                          windowName);
    enableOpenGLDebug();

    hal->setDisplayFunction([]() { activeEngine->display(); });
    hal->setIdleFunction([]() {});
    hal->setResizeFunction([](int x, int y) { activeEngine->resize(x, y); });

    bench("init resource manager"),
        resourceManager.reset(new ResourceManager());

    setClearColor(clear_color);
    glEnable(GL_DEPTH_TEST);

    resourceManager->getAnimationManager().init();
    gui_layer.setVisible(true);
}

Engine::~Engine() {}

void Engine::run()
{
    PrintGLParams();

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

    enableBlend(blend_enabled);
    onDraw();
    gui().draw();
    gui().processInputEvents(getInput());

    hal->swap();
    const auto t2 = std::chrono::system_clock::now();
    const auto dt = t2 - t1;
    const auto milis =
        static_cast<float>(
            std::chrono::duration_cast<std::chrono::microseconds>(dt).count()) /
        1000;
    currentfps = 1000.0f / milis;
}

void Engine::initialSetup() {}
void Engine::onDraw() {}
void Engine::onResize(int x, int y) { (void)x, (void)y; }

const Viewport& Engine::getViewport() const { return viewport; }

Engine& Engine::getActiveEngine() { return *activeEngine; }
HAL& Engine::getHAL() { return *getActiveEngine().hal; }

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
void Engine::setBlend(bool blend) { blend_enabled = blend; }
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

MaterialManager& getMaterialManager()
{
    return Engine::getActiveEngine().getResourceManager().getMaterialManager();
}

void Engine::load(const vvv::CfgNode& node)
{
    getTextureManager().load(node);
    getAnimationManager().load(node);
    getMaterialManager().load(node);
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

void Engine::setClearColor(const vvv3d::Color& color)
{
    clear_color = color;
    glClearColor(color.r, color.g, color.b, color.a);
}
const vvv3d::Color& Engine::getClearColor() const { return clear_color; }

} // namespace vvv3d
