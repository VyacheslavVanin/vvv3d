#pragma once
#include <memory>
#include <vector>

#include <vvv3d/core/camera.hpp>
#include <vvv3d/gui/rect.hpp>
#include <vvv3d/vvvmath/matrices_types.hpp>

namespace vvv3d {
class GuiLayerBase;

class Widget {
public:
    explicit Widget();
    Widget(Widget&&) noexcept = delete;
    Widget& operator=(Widget&&) noexcept = delete;

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
    void setWidth(int width);
    void setHeight(int height);

    const vvv::vector2i& getMinSize() const;
    const vvv::vector2i& getMaxSize() const;
    void setMinSize(int width, int height);
    void setMinSize(const vvv::vector2i& size);
    void setMaxSize(int width, int height);
    void setMaxSize(const vvv::vector2i& size);
    void setMaxWidth(int width);
    void setMaxHeight(int height);
    int getMaxWidth() const;
    int getMaxHeight() const;
    void setMinWidth(int width);
    void setMinHeight(int height);
    int getMinWidth() const;
    int getMinHeight() const;
    /** @brief Resize to minimum size */
    void minimize();

    const Rect& getRect() const;

    void notifyPointerMove(int x, int y);
    void notifyButtonPressed(int button, int x, int y);
    void notifyButtonReleased(int button, int x, int y);
    void notifyTextEntered(const std::string& text);
    void notifyKeyDown(uint16_t scancode);
    void notifyKeyUp(uint16_t scancode);
    /** @brief Notify widget that its content changed (particulary: size of
     * children widget) */
    void notifyContentChanged();

    static Widget* getCurrentFocus();
    bool isFocusable() const;
    void setFocusable(bool focusable);
    void setFocus();

    bool isVisible() const;
    void setVisible(bool visible);
    void toggleVisible();

    virtual void loadResources();

private:
    static Widget* focus;
    bool focusable = false;
    bool visible = true;
    bool ignore_content_changed = false;

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

    virtual void onPointerEnter(int x, int y);
    virtual void onPointerLeave(int x, int y);
    virtual void onPointerMove(int x, int y);
    virtual void onButtonPressed(int button, int x, int y);
    virtual void onButtonReleased(int button, int x, int y);
    virtual void onGetFocus();
    virtual void onLoseFocus();
    virtual void onKeyDown(uint16_t scancode);
    virtual void onKeyUp(uint16_t scancode);
    virtual void onTextEntered(const std::string& text);
    virtual void onContentChanged();

    friend class GuiLayerBase;
    void setGuiLayer(GuiLayerBase* layer);
    friend class GuiPointer;

    vvv::vector2i pos;
    vvv::vector2i size;
    vvv::vector2i minSize;
    vvv::vector2i maxSize;

    Rect clipArea;
    Widget* obj;
    Widget* parent;
    GuiLayerBase* layer;
    std::vector<Widget*> children;
    bool hover;
    void updateClipArea();
    bool isIgnoreContentChanged() const;
    void setIgnoreContentChanged(bool ignore);
};
} // namespace vvv3d
