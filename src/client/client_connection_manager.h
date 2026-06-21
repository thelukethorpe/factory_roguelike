#ifndef CLIENT_CONNECTION_MANAGER_H
#define CLIENT_CONNECTION_MANAGER_H

#include <core/op/client_op.h>
#include <core/op/server_op.h>
#include <core/transport/transport.h>
#include <core/util/byte_reader.h>

template <typename TClientTransport> class ClientConnectionManager
{
  public:
    ClientConnectionManager(const typename TClientTransport::Args &client_transport_args)
        : client_transport_(TClientTransport{client_transport_args, {}}) // TODO Callbacks
    {
    }
    ~ClientConnectionManager() = default;

    ClientConnectionManager(const ClientConnectionManager &) = delete;
    ClientConnectionManager &operator=(const ClientConnectionManager &) = delete;

    ClientConnectionManager(ClientConnectionManager &&) = delete;
    ClientConnectionManager &operator=(ClientConnectionManager &&) = delete;

    [[nodiscard]] ClientConnectionId connectionId() const noexcept
    {
        return client_transport_.connectionId();
    }

    void tick() { client_transport_.tick(); }

    template <ClientOp Op> void sendClientOpToServer(const typename Op::Payload &payload)
    {
        const auto serializedOp = serializeOp(Op::id, payload);
        client_transport_.send(serializedOp);
    }

    void receiveOpsFromServer(const ServerOpCallbacks &cbs)
    {
        const auto data = client_transport_.receive();
        ByteReader reader(data);
        while (!reader.empty())
        {
            this->receiveOpFromServer(cbs, reader);
        }
    }

  private:
    TClientTransport client_transport_;

    void receiveOpFromServer(const ServerOpCallbacks &cbs, ByteReader &reader)
    {
        const auto op = reader.read<ServerOpId>();
        switch (op)
        {
        case ServerOpId::SpawnEntity:
            this->dispatchOpFromServer<ServerSpawnEntityOp>(cbs, reader);
            break;
        }
    }

    template <ServerOp Op>
    void dispatchOpFromServer(const ServerOpCallbacks &cbs, ByteReader &reader)
    {
        const auto payload = reader.read<typename Op::Payload>();
        Op::invoke(cbs, payload);
    }
};

#endif
