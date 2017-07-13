#ifndef GUIOBJECT_H
#define GUIOBJECT_H
#include "rect.h"
#include <memory>
#include <vector>
#include <vvv3d/core/camera.h>
#include <vvv3d/vvvmath/matrices_types.h>

namespace vvv3d {
class GuiLayer;

class Widget {
public:
    explicit Widget();
    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;

    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;

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

    void invokePointerMove(int x, int y);
    void invokeButtonPressed(int button, int x, int y);
    void invokeButtonReleased(int button, int x, int y);
    void invokeTextEntered(const std::string& text);
    void invokeKeyDown(uint16_t scancode);
    void invokeKeyUp(uint16_t scancode);
    /** @brief Notify widget that its content changed (particulary: size of
     * children widget) */
    void notifyContentChanged();

    static Widget* getCurrentFocus();
    bool isFocusable() const;
    void setFocusable(bool focusable);
    void setFocus();

    bool isVisible() const;
    void setVisible(bool visible);
    void toggleVisibility();

private:
    static Widget* focus;
    bool focusable = false;
    bool visible   = true;

protected:
    const vvv3d::Camera& getCamera() const;
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
    virtual void OnKeyDown(uint16_t scancode);
    virtual void OnKeyUp(uint16_t scancode);
    virtual void OnTextEntered(const std::string& text);
    virtual void OnContentChanged();

    friend class GuiLayer;
    void setGuiLayer(GuiLayer* layer);
    friend class GuiPointer;

    vvv::vector2i pos;
    vvv::vector2i size;
    vvv::vector2i minSize;
    vvv::vector2i maxSize;

    Rect clipArea;
    Widget* obj;
    Widget* parent;
    GuiLayer* layer;
    std::vector<Widget*> children;
    bool hover;
    void updateClipArea();
};
}

#endif // GUIOBJECT_H
