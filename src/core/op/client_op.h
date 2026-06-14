#ifndef CLIENT_OP_H
#define CLIENT_OP_H

#include "op.h"
#include <functional>

enum ClientOpId : OpId
{
    Move,
};

template <typename Op>
concept ClientOp = requires {
    typename Op::Payload;
    { Op::id } -> std::same_as<const ClientOpId &>;
};

struct ClientMoveOpPayload
{
    double x, y;
};

struct ClientOpCallbacks
{
    std::function<void(const ClientMoveOpPayload &)> onMove;
};

struct ClientMoveOp
{
    static constexpr ClientOpId id = ClientOpId::Move;

    using Payload = ClientMoveOpPayload;

    static void invoke(const ClientOpCallbacks &cbs, const Payload &p) { cbs.onMove(p); }
};

#endif
