#ifndef GUILAYER_H
#define GUILAYER_H
#include <memory>

class Widget;
class Camera;
class GuiLayer final
{
public:
    GuiLayer();
    ~GuiLayer();
    GuiLayer(GuiLayer&&) noexcept;
    GuiLayer& operator=(GuiLayer&&) noexcept;

    void draw();
    void addWidget(Widget* widget);
    void removeWidget(Widget* widget);
    void resize(int width, int height);

    const Camera& getCamera() const;

private:
    struct GuiLayerImpl;
    std::unique_ptr<GuiLayerImpl> impl;
};

#endif // GUILAYER_H
