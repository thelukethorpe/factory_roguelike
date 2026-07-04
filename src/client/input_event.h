#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include <cstdint>

enum InputEventType : std::uint8_t
{
    MoveNorthKey,
    MoveEastKey,
    MoveSouthKey,
    MoveWestKey,
};

struct KeyEvent
{
    struct Args
    {
        bool is_key_down;
    };

    InputEventType type;
    Args args;
};

#endif
