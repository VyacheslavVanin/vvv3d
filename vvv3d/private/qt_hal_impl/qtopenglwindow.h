#pragma once

#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QOpenGLPaintDevice>
#include <QWindow>

#include "qtinput.hpp"

class QtOpenGLWindow : public QWindow, protected QOpenGLExtraFunctions {
    Q_OBJECT
public:
    explicit QtOpenGLWindow(QWindow* parent = nullptr);
    ~QtOpenGLWindow() override;

    virtual void render(QPainter* painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

    void setResizeFunction(const std::function<void(int x, int y)>& function);
    void setDisplayFunction(const std::function<void()>& function);
    void setIdleFunction(const std::function<void()>& function);
    void setVSync(bool enable);

    const QtLayerInput& GetInput() const { return qt_input; }

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent* event) override;

    void exposeEvent(QExposeEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    bool m_animating;

    QOpenGLContext* m_context;
    QOpenGLPaintDevice* m_device;

    std::function<void(int x, int y)> resize_function;
    std::function<void()> display_function;
    std::function<void()> idle_function;

    QtLayerInput qt_input;
};
