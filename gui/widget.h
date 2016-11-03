#ifndef GUIOBJECT_H
#define GUIOBJECT_H
#include <memory>
#include <vvv3d/vvvmath/matrices_types.h>

class GuiLayer;
class Camera;
class Widget
{
public:
    Widget(Widget* parent = nullptr);
    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;

    virtual ~Widget();

    void Draw();

    const vvv::vector2i& getPosition() const;
    const vvv::vector2i getAbsolutePosition() const;
    void setPosition(const vvv::vector2i& newPos);
    void setPosition(int x, int y);

    const vvv::vector2i& getSize() const;
    void setSize(const vvv::vector2i& size);
    void setSize(int width, int height);

    const vvv::vector2i& getMinSize() const;
    const vvv::vector2i& getMaxSize() const;
    void setMinSize(int width, int height);
    void setMinSize(const vvv::vector2i& size);
    void setMaxSize(int width, int height);
    void setMaxSize(const vvv::vector2i& size);


    void setParent(Widget* parent);
    Widget* getParent() const;
    void addWidget(Widget* widget);
    void removeWidget(Widget* widget);

protected:
    const Camera& getCamera() const;
    /**
     * @brief Implement to draw something */
    virtual void onDraw();

    /**
     * @brief Called after setSize()
     * @param oldSize
     * @param newSize */
    virtual void onResize(const vvv::vector2i& oldSize,
                          const vvv::vector2i& newSize);

    virtual void onAddWidget(Widget* added);
    virtual void onRemoveWidget(Widget* removed);

private:
    friend class GuiLayer;
    void setGuiLayer(GuiLayer* layer);

    struct WidgetImpl;
    std::unique_ptr<WidgetImpl> impl;
};

#endif // GUIOBJECT_H
