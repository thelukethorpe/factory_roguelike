#include "client_connection_manager.h"

template <typename TClientTransport, typename TClientTransportArgs>
template <ServerOp Op>
void ClientConnectionManager<TClientTransport, TClientTransportArgs>::sendServerOp(
    const typename Op::Payload &payload)
{
    const auto serializedOp = serializeOp(Op::id, payload);
    client_transport_.send(serializedOp);
}

template <typename TClientTransport, typename TClientTransportArgs>
void ClientConnectionManager<TClientTransport, TClientTransportArgs>::receiveServerOps(
    const ServerOpCallbacks &cbs)
{
    const auto data = client_transport_.receive();
    ByteReader reader(data);
    while (!reader.empty())
    {
        this->receiveServerOp(cbs, reader);
    }
};

template <typename TClientTransport, typename TClientTransportArgs>
void ClientConnectionManager<TClientTransport, TClientTransportArgs>::receiveServerOp(
    const ServerOpCallbacks &cbs, ByteReader &reader)
{
    const auto op = reader.read<ServerOpId>();
    switch (op)
    {
    case ServerOpId::SpawnEntity:
        this->dispatchServerOp<ServerSpawnEntityOp>(cbs, reader);
        break;
    }
}

template <typename TClientTransport, typename TClientTransportArgs>
template <ServerOp Op>
void ClientConnectionManager<TClientTransport, TClientTransportArgs>::dispatchServerOp(
    const ServerOpCallbacks &cbs, ByteReader &reader)
{
    const auto payload = reader.read<typename Op::Payload>();
    Op::invoke(cbs, payload);
}
