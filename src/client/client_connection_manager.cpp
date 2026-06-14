#include "client_connection_manager.h"

template <typename TClientTransport>
template <ServerOp Op>
void ClientConnectionManager<TClientTransport>::sendServerOp(const typename Op::Payload &payload)
{
    const auto serializedOp = serializeOp(Op::id, payload);
    client_transport_.send(serializedOp);
}

template <typename TClientTransport>
void ClientConnectionManager<TClientTransport>::receiveServerOps(const ServerOpCallbacks &cbs)
{
    const auto data = client_transport_.receive();
    ByteReader reader(data);
    while (!reader.empty())
    {
        this->receiveServerOp(cbs, reader);
    }
};

template <typename TClientTransport>
void ClientConnectionManager<TClientTransport>::receiveServerOp(const ServerOpCallbacks &cbs,
                                                                ByteReader &reader)
{
    const auto op = reader.read<ServerOpId>();
    switch (op)
    {
    case ServerOpId::SpawnEntity:
        this->dispatchServerOp<ServerSpawnEntityOp>(cbs, reader);
        break;
    }
}

template <typename TClientTransport>
template <ServerOp Op>
void ClientConnectionManager<TClientTransport>::dispatchServerOp(const ServerOpCallbacks &cbs,
                                                                 ByteReader &reader)
{
    const auto payload = reader.read<typename Op::Payload>();
    Op::invoke(cbs, payload);
}
