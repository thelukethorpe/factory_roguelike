#include "server_connection_manager.h"

template <typename TServerTransport>
template <ClientOp Op>
void ServerConnectionManager<TServerTransport>::sendClientOp(ClientConnectionId client_id,
                                                             const typename Op::Payload &payload)
{
    const auto serializedOp = serializeOp(Op::id, payload);
    server_transport_.send(client_id, serializedOp);
}

template <typename TServerTransport>
void ServerConnectionManager<TServerTransport>::receiveClientOps(ClientConnectionId client_id,
                                                                 const ClientOpCallbacks &cbs)
{
    const auto data = server_transport_.receive(client_id);
    ByteReader reader(data);
    while (!reader.empty())
    {
        this->receiveClientOp(reader, cbs);
    }
};

template <typename TServerTransport>
void ServerConnectionManager<TServerTransport>::receiveClientOp(ByteReader &reader,
                                                                const ClientOpCallbacks &cbs)
{

    const auto op = reader.read<ClientOpId>();
    switch (op)
    {
    case ClientOpId::Move:
        this->dispatchClientOp<ClientMoveOp>(reader, cbs);
        break;
    }
};

template <typename TServerTransport>
template <typename Op>
void ServerConnectionManager<TServerTransport>::dispatchClientOp(ByteReader &reader,
                                                                 const ClientOpCallbacks &cbs)
{
    const auto payload = reader.read<typename Op::Payload>();
    Op::invoke(cbs, payload);
}
