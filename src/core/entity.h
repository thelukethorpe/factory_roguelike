#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>

using EntityId = std::uint32_t;

enum EntityType : std::uint8_t
{
    Monster,
};

#endif
