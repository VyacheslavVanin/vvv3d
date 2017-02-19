#ifndef INPUTEVENT_H
#define INPUTEVENT_H
#include <stdint.h>

enum class INPUT_EVENT_TYPE
{
    KEY_DOWN,
    KEY_UP
};

struct InputEvent
{
    InputEvent() {}
    InputEvent(INPUT_EVENT_TYPE type, uint16_t scancode)
        : type(type), scancode(scancode)
    {}
    INPUT_EVENT_TYPE type;
    uint16_t         scancode;
};

#endif
