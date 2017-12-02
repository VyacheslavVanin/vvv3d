#pragma once
#include <stdint.h>

namespace vvv3d {

enum class INPUT_EVENT_TYPE { KEY_DOWN, KEY_UP };

enum SCANCODE {
    SCANCODE_ENTER     = 40,
    SCANCODE_ESC       = 41,
    SCANCODE_BACKSPACE = 42,
    SCANCODE_LEFT      = 80,
    SCANCODE_RIGHT     = 79,
    SCANCODE_DOWN      = 81,
    SCANCODE_UP        = 82,
    SCANCODE_DELETE    = 76
};

struct InputEvent {
    InputEvent() {}
    InputEvent(INPUT_EVENT_TYPE type, uint16_t scancode)
        : type(type), scancode(scancode)
    {
    }

    bool operator==(const InputEvent& other) const
    {
        return type == other.type && scancode == other.scancode;
    }

    INPUT_EVENT_TYPE type;
    uint16_t scancode;
};
}

