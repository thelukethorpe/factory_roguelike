#ifndef KIT_H
#define KIT_H

#include <cstdint>

enum LoadoutId : std::uint8_t
{
    Warper,
    NumLoadouts
};

struct Loadout
{
    LoadoutId id;
    double speed;
};

#endif
