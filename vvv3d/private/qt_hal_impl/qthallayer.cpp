#include "qthallayer.hpp"

#include <vvv3d/core/graphics/lowlevel/openglprovider.hpp>

#include "qt_font_impl.h"

#include <QApplication>
#include <QEventLoop>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>

#include <QFont>
#include <QFontDatabase>
#include <QDebug>

namespace vvv3d {
QtHalLayer::QtHalLayer() {}

QtHalLayer::~QtHalLayer() { delete window; }

void QtHalLayer::initContext(int argc, char** argv)
{
    this->argc = argc;
    this->argv = argv;
    app = std::make_unique<QApplication>(this->argc, this->argv);
}

void QtHalLayer::createWindow(int width, int height, const char* name)
{
    window = new QtOpenGLWindow();
    window->resize(width, height);
    window->setTitle(name);

    QEventLoop event_loop;
    window->setDisplayFunction([&event_loop] { event_loop.exit(); });
    window->show();

    event_loop.exec();
}

void QtHalLayer::setResizeFunction(
    const std::function<void(int, int)>& resizeFunction)
{
    window->setResizeFunction(resizeFunction);
    resizeFunction(window->width(), window->height());
}

void QtHalLayer::setDisplayFunction(
    const std::function<void()>& displayFunction)
{
    window->setDisplayFunction(displayFunction);
}

void QtHalLayer::setIdleFunction(const std::function<void()>& idleFunction)
{
    window->setIdleFunction(idleFunction);
}

void QtHalLayer::mainLoop() { app->exec(); }

void QtHalLayer::stopMainLoop()
{
    app->exit();
}

void QtHalLayer::swap()
{
    // unused
}

size_t QtHalLayer::getNumScanCodes() const
{
    return window->GetInput().MAX_SCANCODES;
}

bool QtHalLayer::keyDown(uint16_t scancode) const
{
    return window->GetInput().keyDown(scancode);
}

bool QtHalLayer::mouseButtonDown(uint16_t button) const
{
    return window->GetInput().mouseButtonDown(button);
}

int QtHalLayer::getMouseX() const { return window->GetInput().getMouseX(); }

int QtHalLayer::getMouseY() const { return window->GetInput().getMouseY(); }

int QtHalLayer::getMouseRelX() const
{
    return window->GetInput().getMouseRelX();
}

int QtHalLayer::getMouseRelY() const
{
    return window->GetInput().getMouseRelY();
}

bool QtHalLayer::hasText() const { return window->GetInput().hasText(); }

const std::string& QtHalLayer::getText() const
{
    return window->GetInput().getText();
}

const std::vector<vvv3d::InputEvent>& QtHalLayer::getEvents() const
{
    return window->GetInput().getInputEvents();
}

void QtHalLayer::setVSync(bool vsync) { window->setVSync(vsync); }

vvv3d::LowLevelTexture*
QtHalLayer::readTexture(const std::string& filename) const
{
    QImageReader ir(filename.c_str());
    // TODO: check not flipped
    const auto image =
        ir.read().convertToFormat(QImage::Format_RGBA8888).mirrored();
    const auto* data = image.bits();
    const GLuint w = static_cast<GLuint>(image.width());
    const GLuint h = static_cast<GLuint>(image.height());
    return new vvv3d::LowLevelTexture(data, w, h, GL_RGBA, GL_RGBA8,
                                      GL_UNSIGNED_BYTE);
}

void QtHalLayer::writeTexture(const std::string& filename,
                              const vvv3d::LowLevelTexture* llt, uint32_t width,
                              uint32_t height, uint32_t offsetx,
                              uint32_t offsety) const
{
    static constexpr auto channel_count = 4;
    const auto buf_size = width * height * channel_count;

    std::vector<uint8_t> buf(buf_size);
    vvv3d::readImage(llt, buf.data(), GL_RGBA, GL_UNSIGNED_BYTE, width, height,
                     offsetx, offsety);

    QImage im(buf.data(), (int)width, (int)height, QImage::Format_RGBA8888);

    QImageWriter iw(filename.c_str());
    iw.write(im.mirrored());
}

std::unique_ptr<vvv3d::IFont> QtHalLayer::GetFont(const std::string& font_name,
                                                  unsigned pixel_size,
                                                  unsigned char_size,
                                                  unsigned dpi) const
{
    auto id = QFontDatabase::addApplicationFont(font_name.c_str());
    auto family = QFontDatabase::applicationFontFamilies(id)[0];
    QFont font(family, (int)char_size);
    return std::make_unique<QtFontImpl>(font, char_size, pixel_size, dpi);
}

std::unique_ptr<vvv3d::IFont> QtHalLayer::GetFont(const FontDesc& desc,
                                                  unsigned pixel_size,
                                                  unsigned char_size,
                                                  unsigned dpi) const
{
    const auto& filename = desc.getFileName();
    if (filename.size())
        return GetFont(filename, pixel_size, char_size, dpi);

    QFontDatabase font_db;
    const auto& font = font_db.font(desc.getFamily().c_str(),
                                    desc.getStyle().c_str(), char_size);
    return std::make_unique<QtFontImpl>(font, char_size, pixel_size, dpi);
}

// SystemFonts implementation usen Qt TODO: Move to sepaarate files
namespace {
class QtSystemFontsImpl : public vvv3d::ISystemFonts {
public:
    QtSystemFontsImpl();
    const std::vector<vvv3d::FontDesc>& getFontDescList() const override;

private:
    std::vector<vvv3d::FontDesc> font_list;
};

std::vector<vvv3d::FontDesc> makeFontDescList()
{
    const static std::string empty;
    std::vector<vvv3d::FontDesc> ret;
    QFontDatabase db;
    for (const auto& family : db.families()) {
        for (const auto& style : db.styles(family)) {
            vvv3d::FontDesc desc(empty, family.toStdString(),
                                 style.toStdString(), empty);
            ret.push_back(desc);
        }
    }

    return ret;
}

QtSystemFontsImpl::QtSystemFontsImpl() : font_list(makeFontDescList())
{
    // Required to provide getDefault...() methods
    loadDefaultFonts();
}

const std::vector<vvv3d::FontDesc>& QtSystemFontsImpl::getFontDescList() const
{
    return font_list;
}
} // namespace

vvv3d::ISystemFonts& QtHalLayer::GetSystemFonts()
{
    static QtSystemFontsImpl instance;
    return instance;
}

void QtHalLayer::showVirtualKeyboard(bool show)
{
    if (show)
        app->inputMethod()->show();
    else
        app->inputMethod()->hide();
}

std::string QtHalLayer::toUtf8(const std::u32string& u32) const
{
    return QString::fromStdU32String(u32).toStdString();
}

std::u32string QtHalLayer::toUtf32(const std::string& u8) const
{
    return QString::fromStdString(u8).toStdU32String();
}

void QtHalLayer::log(const char* str)
{
    qDebug() << str;
}
} // namespace vvv3d
