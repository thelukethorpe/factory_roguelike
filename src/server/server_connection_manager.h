#ifndef SERVER_CONNECTION_MANAGER_H
#define SERVER_CONNECTION_MANAGER_H

#include <core/byte_reader.h>
#include <core/op/client_op.h>
#include <core/transport/transport.h>

struct ServerConnectionManagerCallbacks
{
    std::function<void(ClientConnectionId client_id, double x, double y)> onClientMove;
};

template <typename TServerTransport, typename TServerTransportArgs> class ServerConnectionManager
{
  public:
    ServerConnectionManager(const TServerTransportArgs &serverTransportArgs,
                            ServerConnectionManagerCallbacks callbacks)
        : server_transport_(TServerTransport{serverTransportArgs}), callbacks_(std::move(callbacks))
    {
    }
    ~ServerConnectionManager() = default;

    ServerConnectionManager(const ServerConnectionManager &) = delete;
    ServerConnectionManager &operator=(const ServerConnectionManager &) = delete;

    ServerConnectionManager(ServerConnectionManager &&) = delete;
    ServerConnectionManager &operator=(ServerConnectionManager &&) = delete;

    template <ClientOp Op>
    void sendClientOp(ClientConnectionId client_id, const typename Op::Payload &payload);

    void receiveClientOps(ClientConnectionId client_id, const ClientOpCallbacks &cbs);

  private:
    TServerTransport server_transport_;
    ServerConnectionManagerCallbacks callbacks_;

    void receiveClientOp(ByteReader &reader, const ClientOpCallbacks &cbs);

    template <typename Op> void dispatchClientOp(ByteReader &reader, const ClientOpCallbacks &cbs);
};

#endif
