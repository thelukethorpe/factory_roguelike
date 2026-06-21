#ifndef SERVER_OP_H
#define SERVER_OP_H

#include "op.h"
#include <core/game/entity.h>
#include <functional>

enum ServerOpId : OpId
{
    SpawnEntity,
};

template <typename Op>
concept ServerOp = requires {
    typename Op::Payload;
    { Op::id } -> std::same_as<const ServerOpId &>;
};

struct ServerSpawnEntityOpPayload
{
    EntityId id;
    EntityType type;
    double x, y;
};

struct ServerOpCallbacks
{
    std::function<void(const ServerSpawnEntityOpPayload &)> onSpawnEntity;
};

struct ServerSpawnEntityOp
{
    static constexpr ServerOpId id = ServerOpId::SpawnEntity;

    using Payload = ServerSpawnEntityOpPayload;

    static void invoke(const ServerOpCallbacks &cbs, const Payload &p) { cbs.onSpawnEntity(p); }
};

#endif
