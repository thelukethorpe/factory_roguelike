#ifndef SERVER_CONNECTION_MANAGER_H
#define SERVER_CONNECTION_MANAGER_H

#include <core/byte_reader.h>
#include <core/op/client_op.h>
#include <core/op/server_op.h>
#include <core/transport/transport.h>

struct ServerConnectionManagerCallbacks
{
    std::function<void(ClientConnectionId client_id, double x, double y)> onClientMove;
};

template <typename TServerTransport> class ServerConnectionManager
{
  public:
    ServerConnectionManager(const typename TServerTransport::Args &server_transport_args)
        : server_transport_(TServerTransport{server_transport_args, {}}) // TODO Callbacks
    {
    }
    ~ServerConnectionManager() = default;

    ServerConnectionManager(const ServerConnectionManager &) = delete;
    ServerConnectionManager &operator=(const ServerConnectionManager &) = delete;

    ServerConnectionManager(ServerConnectionManager &&) = delete;
    ServerConnectionManager &operator=(ServerConnectionManager &&) = delete;

    [[nodiscard]] ServerConnectionId connectionId() const noexcept
    {
        return server_transport_.connectionId();
    }

    void tick() { server_transport_.tick(); }

    template <ServerOp Op>
    void sendServerOpToClient(ClientConnectionId client_id, const typename Op::Payload &payload)
    {
        const auto serializedOp = serializeOp(Op::id, payload);
        server_transport_.send(client_id, serializedOp);
    }

    void receiveOpsFromClient(ClientConnectionId client_id, const ClientOpCallbacks &cbs)
    {
        const auto data = server_transport_.receive(client_id);
        ByteReader reader(data);
        while (!reader.empty())
        {
            this->receiveOpFromClient(reader, cbs);
        }
    }

  private:
    TServerTransport server_transport_;

    void receiveOpFromClient(ByteReader &reader, const ClientOpCallbacks &cbs)
    {
        const auto op = reader.read<ClientOpId>();
        switch (op)
        {
        case ClientOpId::Move:
            this->dispatchOpFromClient<ClientMoveOp>(reader, cbs);
            break;
        }
    }

    template <ClientOp Op>
    void dispatchOpFromClient(ByteReader &reader, const ClientOpCallbacks &cbs)
    {
        const auto payload = reader.read<typename Op::Payload>();
        Op::invoke(cbs, payload);
    }
};

#endif
