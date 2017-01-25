#ifndef GUIOBJECT_H
#define GUIOBJECT_H
#include <memory>
#include <vector>
#include <vvv3d/vvvmath/matrices_types.h>
#include "rect.h"

class GuiLayer;
class Camera;
class Widget
{
public:
    explicit Widget();
    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;

    virtual ~Widget();

    void Draw();

    const vvv::vector2i& getPosition() const;
    const vvv::vector2i getAbsolutePosition() const;
    void setPosition(const vvv::vector2i& newPos);
    void setPosition(int x, int y);

    const vvv::vector2i& getSize() const;
    int getWidth() const;
    int getHeight() const;
    void setSize(const vvv::vector2i& size);
    void setSize(int width, int height);

    const vvv::vector2i& getMinSize() const;
    const vvv::vector2i& getMaxSize() const;
    void setMinSize(int width, int height);
    void setMinSize(const vvv::vector2i& size);
    void setMaxSize(int width, int height);
    void setMaxSize(const vvv::vector2i& size);

    const Rect& getRect() const;


protected:
    const Camera& getCamera() const;
    const std::vector<Widget*>& getChildren() const;

    /**
     * @brief Implement to draw something */
    virtual void onDraw();

    /**
     * @brief Called after setSize()
     * @param oldSize
     * @param newSize */
    virtual void onResize(const vvv::vector2i& oldSize,
                          const vvv::vector2i& newSize);

    void setParent(Widget* newParent);
    Widget* getParent() const;
    bool addChild(Widget* widget);
    bool removeChild(Widget* child);

    virtual void OnPointerEnter(int x, int y);
    virtual void OnPointerLeave(int x, int y);
    virtual void OnPointerMove(int x, int y);
    virtual void OnButtonPressed(int button, int x, int y);
    virtual void OnButtonReleased(int button, int x, int y);
    virtual void OnGetFocus();
    virtual void OnLoseFocus();

    friend class GuiLayer;
    void setGuiLayer(GuiLayer* layer);
    void invokePointerMove(int x, int y);
    void invokeButtonPressed(int button, int x, int y);
    void invokeButtonReleased(int button, int x, int y);
    friend class GuiPointer;
    void setFocus();
    void setUnfocused();

    vvv::vector2i   pos     {0};
    vvv::vector2i   size    {1};
    vvv::vector2i   minSize {1};
    vvv::vector2i   maxSize {INT32_MAX};

    Rect            clipArea;
    Widget*         obj     {nullptr};
    Widget*         parent  {nullptr};
    GuiLayer*       layer   {nullptr};
    std::vector<Widget*> children;
    bool            hover = false;
    void updateClipArea();
};

#endif // GUIOBJECT_H
