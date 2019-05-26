#include "qtopenglprovider.hpp"
#include "qtopenglwindow.h"
#include <QDebug>
#include <QPainter>
#include <QResizeEvent>

#include <QInputEvent>
#include <QKeyEvent>
#include <QMouseEvent>

QtOpenGLWindow::QtOpenGLWindow(QWindow* parent)
    : QWindow(parent), m_animating(true), m_context(nullptr), m_device(nullptr)
{
    resize_function = [](int, int) {};
    display_function = [] {};
    idle_function = [] {};
    setSurfaceType(QWindow::OpenGLSurface);
    QSurfaceFormat format;
    format.setSwapInterval(1);
    format.setRenderableType(QSurfaceFormat::OpenGLES);
    format.setDepthBufferSize(24);
    format.setVersion(3, 2);

    setFormat(format);
}

QtOpenGLWindow::~QtOpenGLWindow() {}

void QtOpenGLWindow::render(QPainter* painter) { std::ignore = painter; }

void QtOpenGLWindow::render()
{
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    m_device->setSize(size() * devicePixelRatio());
    m_device->setDevicePixelRatio(devicePixelRatio());

    idle_function();
    display_function();

    QPainter painter(m_device);
    render(&painter);
}

void QtOpenGLWindow::initialize() {}

void QtOpenGLWindow::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating)
        renderLater();
}

void QtOpenGLWindow::setResizeFunction(
    const std::function<void(int, int)>& function)
{
    resize_function = function;
}

void QtOpenGLWindow::setDisplayFunction(const std::function<void()>& function)
{
    display_function = function;
}

void QtOpenGLWindow::setIdleFunction(const std::function<void()>& function)
{
    idle_function = function;
}

void QtOpenGLWindow::setVSync(bool enable)
{
    auto f = format();
    f.setSwapInterval(enable);
    setFormat(f);
}

void QtOpenGLWindow::renderLater() { requestUpdate(); }

void QtOpenGLWindow::renderNow()
{
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize) {
        initializeOpenGLFunctions();
        __vvv3d_qt_opengl_funcs = this;
        initialize();
    }

    render();

    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();
}

bool QtOpenGLWindow::event(QEvent* event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest: renderNow(); qt_input.clear(); return true;

    case QEvent::InputMethod:
    case QEvent::InputMethodQuery:
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease: qt_input.FeedEvent(event); return true;

    default: return QWindow::event(event);
    }
}

void QtOpenGLWindow::exposeEvent(QExposeEvent* event)
{
    Q_UNUSED(event)

    if (isExposed())
        renderNow();
}

void QtOpenGLWindow::resizeEvent(QResizeEvent* event)
{
    QWindow::resizeEvent(event);

    const auto& size = event->size();
    const auto& width = size.width();
    const auto& height = size.height();
    resize_function(width, height);
}
