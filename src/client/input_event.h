#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include <cstdint>

enum InputEventType : std::uint8_t
{
    MoveNorthKeyUp,
    MoveNorthKeyDown,
    MoveEastKeyUp,
    MoveEastKeyDown,
    MoveSouthKeyUp,
    MoveSouthKeyDown,
    MoveWestKeyUp,
    MoveWestKeyDown,
};

struct MoveNorthKeyUpEvent
{
    static constexpr InputEventType type = InputEventType::MoveNorthKeyUp;

    struct Args
    {
    };
};

struct MoveNorthKeyDownEvent
{
    static constexpr InputEventType type = InputEventType::MoveNorthKeyDown;

    struct Args
    {
    };
};

#endif
