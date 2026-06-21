#ifndef KIT_H
#define KIT_H

#include <cstdint>

enum LoadoutId : std::uint8_t
{
    Warper
};

struct Loadout
{
    LoadoutId id;
    double speed;
};

#endif
