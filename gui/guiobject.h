#ifndef GUIOBJECT_H
#define GUIOBJECT_H
#include <memory>
#include <vvv3d/vvvmath/matrices_types.h>

class GuiLayer;
class Camera;
class Widget
{
public:
    Widget();
    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;

    virtual ~Widget();

    void Draw();

    const vvv::vector2f& getPosition() const;
    void setPosition(const vvv::vector2f& newPos);

    const vvv::vector2f& getSize() const;
    void setSize(const vvv::vector2f& size);
    void setSize(float width, float height);

    const vvv::vector2f getAbsolutePosition() const;

    void setParent(Widget* parent);
    Widget* getParent() const;
    void addWidget(Widget* widget);
    void removeWidget(Widget* widget);

protected:
    const Camera& getCamera() const;
    /**
     * @brief Implement to draw something */
    virtual void onDraw();
    virtual void onResize(const vvv::vector2f& oldSize,
                          const vvv::vector2f& newSize);

private:
    friend class GuiLayer;
    void setGuiLayer(GuiLayer* layer);

    struct WidgetImpl;
    std::unique_ptr<WidgetImpl> impl;
};

#endif // GUIOBJECT_H
