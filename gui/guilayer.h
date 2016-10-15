#ifndef GUILAYER_H
#define GUILAYER_H
#include <memory>

class GuiObject;

class GuiLayer final
{
public:
    GuiLayer();
    ~GuiLayer();
    GuiLayer(GuiLayer&&) noexcept;
    GuiLayer& operator=(GuiLayer&&) noexcept;

    void draw();
    void addWidget(GuiObject* widget);
    void removeWidget(GuiObject* widget);

private:
    struct GuiLayerImpl;
    std::unique_ptr<GuiLayerImpl> impl;
};

#endif // GUILAYER_H
