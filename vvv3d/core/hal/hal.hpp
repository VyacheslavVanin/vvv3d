#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <vvv3d/core/graphics/lowlevel/lowleveltexture.hpp>
#include <vvv3d/core/hal/fonts/interface/font_interface.hpp>
#include <vvv3d/core/hal/fonts/interface/systemfonts.hpp>
#include <vvv3d/core/inputevent.hpp>
#include <vvv3d/vvvmath/matrices_types.hpp>

namespace vvv3d {

enum class GLPROFILE { ES, CORE };

/**
 * @brief The HAL class. Needed to hide implementation detail such as used
 * libraries (boost/STL/Qt/FreeType and so on)
 *  Interface that provide:<br>
 *  - initialization of OpenGL Context <br>
 *  - Main window creation <br>
 *  - callback functions (resize, display, idle)
 *  - main event loop
 *  - acces to input data (mouse, keyboard etc.)
 */
class HAL {
public:
    virtual ~HAL();
    // Main functionality (context, window, event loop)
    virtual void initContext(int argc, char** argv) = 0;
    virtual void createWindow(int width, int height, const char* name) = 0;
    virtual void setResizeFunction(
        const std::function<void(int x, int y)>& resizeFunction) = 0;
    virtual void
    setDisplayFunction(const std::function<void()>& displayFunction) = 0;
    virtual void setIdleFunction(const std::function<void()>& idleFunction) = 0;
    virtual void mainLoop() = 0;
    virtual void stopMainLoop() = 0;
    virtual void swap() = 0;

    // Input handling (keyboard, mouse, input events)
    virtual size_t getNumScanCodes() const = 0;
    virtual bool keyDown(uint16_t scancode) const = 0;
    virtual bool mouseButtonDown(uint16_t button) const = 0;
    virtual int getMouseX() const = 0;
    virtual int getMouseY() const = 0;
    vvv::vector2i getMousePos() const;
    virtual int getMouseRelX() const = 0;
    virtual int getMouseRelY() const = 0;
    bool isMouseMoved() const;
    vvv::vector2i getMouseRel() const;

    virtual bool hasText() const = 0;
    virtual const std::string& getText() const = 0;
    virtual const std::vector<InputEvent>& getEvents() const = 0;

    virtual void setVSync(bool vsync) = 0;

    // Texture loading/saving
    virtual LowLevelTexture* readTexture(const std::string& filename) const = 0;
    virtual void writeTexture(const std::string& filename,
                              const LowLevelTexture* llt, uint32_t width,
                              uint32_t height, uint32_t offsetx,
                              uint32_t offsety) const = 0;

    // Fonts
    virtual std::unique_ptr<IFont> GetFont(const std::string& font_name,
                                           unsigned pixel_size,
                                           unsigned char_size,
                                           unsigned dpi) const = 0;
    virtual std::unique_ptr<IFont> GetFont(const vvv3d::FontDesc& desc,
                                           unsigned pixel_size,
                                           unsigned char_size,
                                           unsigned dpi) const = 0;
    // Get object storing fonts list
    virtual const ISystemFonts& GetSystemFonts() const = 0;

    // Show virtual keyboard. May be ignored if not provided by implementation
    virtual void showVirtualKeyboard(bool show);

    // String conversions
    /// Convert Utf32 (Ucs4) to Utf8
    virtual std::string toUtf8(const std::u32string& input) const = 0;
    /// Convert Utf8 to Utf32 (Ucs4)
    virtual std::u32string toUtf32(const std::string& input) const = 0;

    virtual void log(const char*);
};
} // namespace vvv3d
