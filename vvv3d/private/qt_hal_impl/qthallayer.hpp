#pragma once
#include <memory>
#include <vvv3d/core/hal/hal.hpp>

#include "qtopenglwindow.h"
#include <QApplication>

namespace vvv3d {

class QtHalLayer : public vvv3d::HAL {
public:
    QtHalLayer();
    ~QtHalLayer();

    // HAL interface
public:
    void initContext(int argc, char** argv) override;
    void createWindow(int width, int height, const char* name) override;
    void setResizeFunction(
        const std::function<void(int, int)>& resizeFunction) override;
    void
    setDisplayFunction(const std::function<void()>& displayFunction) override;
    void setIdleFunction(const std::function<void()>& idleFunction) override;
    void mainLoop() override;
    void stopMainLoop() override;
    void swap() override;
    size_t getNumScanCodes() const override;
    bool keyDown(uint16_t scancode) const override;
    bool mouseButtonDown(uint16_t button) const override;
    int getMouseX() const override;
    int getMouseY() const override;
    int getMouseRelX() const override;
    int getMouseRelY() const override;
    bool hasText() const override;
    const std::string& getText() const override;
    const std::vector<vvv3d::InputEvent>& getEvents() const override;
    void setVSync(bool vsync) override;

    vvv3d::LowLevelTexture*
    readTexture(const std::string& filename) const override;
    void writeTexture(const std::string& filename,
                      const vvv3d::LowLevelTexture* llt, uint32_t width,
                      uint32_t height, uint32_t offsetx,
                      uint32_t offsety) const override;
    std::unique_ptr<vvv3d::IFont> GetFont(const std::string& font_name,
                                          unsigned pixel_size,
                                          unsigned char_size,
                                          unsigned dpi) const override;
    std::unique_ptr<vvv3d::IFont> GetFont(const FontDesc& desc,
                                          unsigned pixel_size,
                                          unsigned char_size,
                                          unsigned dpi) const override;
    vvv3d::ISystemFonts& GetSystemFonts() override;
    void showVirtualKeyboard(bool show) override;

    virtual std::string toUtf8(const std::u32string& input) const override;
    virtual std::u32string toUtf32(const std::string& input) const override;

    virtual DisplayProperties GetDisplayProperies() const override;

    void log(const char *) override;
private:
    std::unique_ptr<QApplication> app;
    QtOpenGLWindow* window = nullptr;
    int argc;
    char** argv;
};
} // namespace vvv3d
