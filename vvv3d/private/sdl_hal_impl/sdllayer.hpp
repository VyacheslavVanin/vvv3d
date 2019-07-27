#pragma once
#include <memory>
#include <vvv3d/core/hal/hal.hpp>

namespace vvv3d {

class sdlLayer : public HAL {
public:
    sdlLayer(int argc, char** argv, GLPROFILE p = GLPROFILE::CORE,
             int major = 3, int minor = 3);
    ~sdlLayer();

    // HAL interface
    void initContext(int argc, char** argv) override;
    void createWindow(int width, int height, const char* name) override;
    void mainLoop() override;
    void stopMainLoop() override;
    void swap() override;

    void setResizeFunction(
        const std::function<void(int x, int y)>& resizeFunction) override;

    void
    setDisplayFunction(const std::function<void()>& displayFunction) override;

    void setIdleFunction(const std::function<void()>& idleFunction) override;

    size_t getNumScanCodes() const override;
    bool keyDown(uint16_t scancode) const override;
    bool mouseButtonDown(uint16_t button) const override;
    int getMouseX() const override;
    int getMouseY() const override;
    int getMouseRelX() const override;
    int getMouseRelY() const override;
    bool hasText() const override;
    void setVSync(bool vsync) override;

    /**
     * @brief Return string of text entered at this cycle
     * @return  */
    const std::string& getText() const override;

    /**
     * @brief Return list of input events occurred at this cycle
     * @return  */
    const std::vector<InputEvent>& getEvents() const override;

    LowLevelTexture* readTexture(const std::string& filename) const override;
    void writeTexture(const std::string& filename, const LowLevelTexture* llt,
                      uint32_t width, uint32_t height, uint32_t offsetx,
                      uint32_t offsety) const override;

    std::unique_ptr<IFont> GetFont(const std::string& font_name,
                                   unsigned pixel_size, unsigned char_size,
                                   unsigned dpi) const override;
    std::unique_ptr<IFont> GetFont(const FontDesc& desc, unsigned pixel_size,
                                   unsigned char_size,
                                   unsigned dpi) const override;

    virtual ISystemFonts& GetSystemFonts() override;

    virtual std::string toUtf8(const std::u32string& input) const override;
    virtual std::u32string toUtf32(const std::string& input) const override;

    virtual DisplayProperties GetDisplayProperies() const override;

private:
    class SDLWraper;
    std::unique_ptr<SDLWraper> sdl;
};
} // namespace vvv3d
