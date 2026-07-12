#ifndef KIT_H
#define KIT_H

#include <cstdint>

#define LOADOUT_VALUES                                                                             \
    F(Warper)                                                                                      \
    F(Commando)

enum LoadoutId : std::uint8_t
{
#define F(name) name,
    LOADOUT_VALUES
#undef F
        NumLoadouts
};

struct Loadout
{
    LoadoutId id;
    double speed;
};

#endif
