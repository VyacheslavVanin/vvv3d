#pragma once
#include <memory>
#include <vector>
#include <vvv3d/core/camera.hpp>
#include <vvv3d/core/input.hpp>
#include <vvv3d/vvvmath/vector2.hpp>

namespace vvv3d {

class Widget;
class GuiLayerBase {
public:
    GuiLayerBase();
    ~GuiLayerBase();
    GuiLayerBase(GuiLayerBase&&) noexcept;
    GuiLayerBase& operator=(GuiLayerBase&&) noexcept;

    void draw();
    void addWidget(Widget* widget);
    void removeWidget(Widget* widget);
    void resize(int width, int height);
    void resize(const vvv::vector2i& size);
    const vvv::vector2<int>& getSize() const;

    const vvv3d::Camera& getCamera() const;

    void processInputEvents(const vvv3d::Input& input);

    void setVisible(bool visible);
    bool isVisible() const;
    void toggleVisible();

private:
    using WidgetsContainer = std::vector<Widget*>;

    class GuiPointer {
    public:
        GuiPointer(WidgetsContainer& widgets) : widgets(&widgets) {}

        void processInput(const vvv3d::Input& input);

        Widget* getWidgetAtPoint(const vvv::vector2i& pos);

        constexpr static uint16_t NUM_MOUSE_BUTTONS = 8;

    private:
        WidgetsContainer* widgets;
        bool mouseButtonsStates[NUM_MOUSE_BUTTONS]{false};
        void detectMouseMove(const vvv3d::Input::Mouse& mouse);
        void detectMouseButtons(const vvv3d::Input::Mouse& mouse);
        void processKeyboard(const vvv3d::Input::Keyboard& kbd);
    };

    WidgetsContainer widgets;
    vvv::vector2<int> size;
    /**
     * @brief Orthographic camera; top-left corner (0,0)
     */
    vvv3d::Camera camera;
    GuiPointer pointer;
    bool visible;
};

} // namespace vvv3d
