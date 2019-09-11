#include "qtinput.hpp"

#include <QDebug>
#include <QInputEvent>
#include <QKeyEvent>
#include <QMouseEvent>

#include <vvvstlhelper.hpp>

vvv3d::INPUT_EVENT_TYPE ToInputEventType(QEvent::Type input)
{
    switch (input) {
    case QEvent::KeyPress: return vvv3d::INPUT_EVENT_TYPE::KEY_DOWN;
    case QEvent::KeyRelease: return vvv3d::INPUT_EVENT_TYPE::KEY_UP;
    default: throw std::logic_error("Unsupported QEvent::Type");
    }
}

vvv3d::SCANCODE ToScancode(const QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape: return vvv3d::SCANCODE_ESC;
    case Qt::Key_Return:
    case Qt::Key_Enter: return vvv3d::SCANCODE_ENTER;
    case Qt::Key_Backspace: return vvv3d::SCANCODE_BACKSPACE;
    case Qt::Key_Delete: return vvv3d::SCANCODE_DELETE;
    case Qt::Key_Up: return vvv3d::SCANCODE_UP;
    case Qt::Key_Down: return vvv3d::SCANCODE_DOWN;
    case Qt::Key_Left: return vvv3d::SCANCODE_LEFT;
    case Qt::Key_Right: return vvv3d::SCANCODE_RIGHT;
    default: return vvv3d::SCANCODE_OTHER;
    }
}

vvv3d::InputEvent ToInputEvent(const QKeyEvent *e)
{
    vvv3d::InputEvent ret;
    ret.type = ToInputEventType(e->type());
    ret.scancode = ToScancode(e);
    return ret;
}

QString Printable(const QString &input)
{
    return filter(input, [](const auto& c) { return c.isPrint(); });
}

QtLayerInput::QtLayerInput() { memset(keys, 0, sizeof(keys)); }

bool QtLayerInput::FeedEvent(QEvent *event)
{
    const auto event_type = event->type();
    switch (event_type) {
    case QEvent::MouseMove: return OnMouseMove(event);
    case QEvent::MouseButtonPress: return OnMousePress(event);
    case QEvent::MouseButtonRelease: return OnMouseRelease(event);
    case QEvent::KeyPress: return OnKeyPressed(event);
    case QEvent::KeyRelease: return OnKeyRelease(event);
    case QEvent::InputMethod: return OnInputMethod(event);
    case QEvent::InputMethodQuery: return OnInputMethodQuery(event);
    default: return true;
    }
    return true;
}

int QtLayerInput::getMouseX() const { return x; }

int QtLayerInput::getMouseY() const { return y; }

int QtLayerInput::getMouseRelX() const { return xrel; }

int QtLayerInput::getMouseRelY() const { return yrel; }

bool QtLayerInput::mouseButtonDown(uint16_t button) const
{
    if (!button)
        return false;
    if (button >= 32)
        return false;

    // to match button numbering with sdl
    const uint32_t button_unified = button - 1;
    const auto ret = mouse_buttons & (1u << button_unified);
    return ret;
}

bool QtLayerInput::hasText() const { return text.size(); }

const std::string &QtLayerInput::getText() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return text;
}

void QtLayerInput::clear()
{
    std::lock_guard<std::mutex> lock(mutex);
    xrel = 0;
    yrel = 0;
    text.clear();
    frame_input_events.clear();
}

bool QtLayerInput::keyDown(uint16_t scancode) const
{
    if (scancode >= MAX_SCANCODES)
        return false;

    return keys[scancode];
}

const std::vector<vvv3d::InputEvent> &QtLayerInput::getInputEvents() const
{
    return frame_input_events;
}

bool QtLayerInput::OnMouseMove(QEvent *event)
{
    auto mme = static_cast<QMouseEvent*>(event);
    const auto& pos = mme->pos();
    xrel += pos.x() - x;
    yrel += pos.y() - y;
    x = pos.x();
    y = pos.y();

    return true;
}

bool QtLayerInput::OnMousePress(QEvent *event)
{
    auto mme = static_cast<QMouseEvent*>(event);
    mouse_buttons |= mme->button();
    return true;
}

bool QtLayerInput::OnMouseRelease(QEvent *event)
{
    auto mme = static_cast<QMouseEvent*>(event);
    mouse_buttons &= ~mme->button();
    return true;
}

bool QtLayerInput::OnKeyPressed(QEvent *event)
{
    auto kbe = static_cast<QKeyEvent*>(event);
    std::lock_guard<std::mutex> lock(mutex);
    frame_input_events.push_back(ToInputEvent(kbe));
    const auto& printable = Printable(kbe->text());
    if (printable.size()) {
        text += printable.toUtf8().toStdString();
    }
    keys[ToScancode(kbe)] = 1;
    return true;
}

bool QtLayerInput::OnKeyRelease(QEvent *event)
{
    auto kbe = static_cast<QKeyEvent*>(event);
    std::lock_guard<std::mutex> lock(mutex);
    frame_input_events.push_back(ToInputEvent(kbe));
    keys[ToScancode(kbe)] = 0;
    return true;
}

bool QtLayerInput::OnInputMethod(QEvent *event)
{
    auto ime = static_cast<QInputMethodEvent*>(event);
    const auto event_text = ime->commitString();
    const auto& printable = Printable(event_text);
    if (printable.size()) {
        text += printable.toUtf8().toStdString();
    }
    return true;
}

bool QtLayerInput::OnInputMethodQuery(QEvent *event)
{
    auto ime = static_cast<QInputMethodQueryEvent*>(event);
    return true;
}
