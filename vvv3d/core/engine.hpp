#pragma once
#include <atomic>
#include <memory>

#include <vvv3d/core/input.hpp>
#include <vvv3d/core/time.hpp>
#include <vvv3d/core/viewport.hpp>
#include <vvv3d/std/gui/widgetindex/gui_layer.hpp>

namespace vvv3d {

class ResourceManager;
class TextureManager;
class FontManager;
class GeometryManager;
class ShaderManager;
class AnimationManager;
class HAL;

class Engine {
public:
    Engine(int argc, char** argv, const char* windowName = "vvv3d");
    virtual ~Engine();

    ResourceManager& getResourceManager();
    const Input& getInput() const;

    void run();
    void stop();

    float getCurrentFps() const;
    const Viewport& getViewport() const;
    void setVSync(bool vsync);
    bool isRunning() const;

    GuiLayer& gui();

    /**
     * @brief Load config
     * @param node */
    void load(const vvv::CfgNode& node);
    void load(std::istream& stream);
    void load(const std::string& string);

    static Engine& getActiveEngine();
    /**
     * @brief Return time at the beginning of current frame.
     * @return seconds */
    static double frameTime();
    static double currentTime();
    static int64_t currentTimeMillis();
    static int64_t currentTimeMicros();

    /**
     * @brief Return time passed between brginning of last frame and current.
     * @return seconds */
    static double getTimeSinceLastFrame();

protected:
    virtual void initialSetup();
    virtual void onDraw();
    virtual void onResize(int x, int y);

    static Engine* activeEngine;

private:
    void display();
    void resize(int x, int y);
    float currentfps;
    Viewport viewport;
    std::unique_ptr<HAL> hal;
    std::unique_ptr<ResourceManager> resourceManager;
    friend class Input;
    Input input;
    static Time<> clock;
    vvv3d::GuiLayer gui_layer;
    std::atomic<bool> is_running{false};
};

TextureManager& getTextureManager();
FontManager& getFontManager();
GeometryManager& getGeometryManager();
ShaderManager& getShaderManager();
AnimationManager& getAnimationManager();

} // namespace vvv3d
