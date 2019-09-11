#pragma once
#include <QEvent>

#include <vector>
#include <mutex>
#include <vvv3d/core/inputevent.hpp>

class QtLayerInput {
public:
    // TODO: Move to inputevent.hpp
    static const constexpr size_t MAX_SCANCODES = 512;
    QtLayerInput();

    bool FeedEvent(QEvent* event);

    int getMouseX() const;
    int getMouseY() const;
    int getMouseRelX() const;
    int getMouseRelY() const;
    bool mouseButtonDown(uint16_t button) const;

    bool hasText() const;
    /// TODO: fix this (may be using DoubleBuffer objects?)
    const std::string& getText() const;
    void clear();

    bool keyDown(uint16_t scancode) const;

    const std::vector<vvv3d::InputEvent>& getInputEvents() const;

private:
    bool OnMouseMove(QEvent* event);

    bool OnMousePress(QEvent* event);
    bool OnMouseRelease(QEvent* event);
    bool OnKeyPressed(QEvent* event);
    bool OnKeyRelease(QEvent* event);

    bool OnInputMethod(QEvent* event);
    bool OnInputMethodQuery(QEvent* event);

    float x = 0, y = 0;
    float xrel = 0, yrel = 0;
    uint32_t mouse_buttons = 0;
    char keys[MAX_SCANCODES];

    /// TODO: Possible need mutex
    mutable std::mutex mutex;
    std::vector<vvv3d::InputEvent> frame_input_events;
    std::string text;
};
