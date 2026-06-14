#ifndef CLIENT_CONNECTION_MANAGER_H
#define CLIENT_CONNECTION_MANAGER_H

#include <core/byte_reader.h>
#include <core/op/server_op.h>

template <typename TClientTransport, typename TClientTransportArgs> class ClientConnectionManager
{
  public:
    ClientConnectionManager(const TClientTransportArgs &clientTransportArgs)
        : client_transport_(TClientTransport{clientTransportArgs})
    {
    }
    ~ClientConnectionManager() = default;

    ClientConnectionManager(const ClientConnectionManager &) = delete;
    ClientConnectionManager &operator=(const ClientConnectionManager &) = delete;

    ClientConnectionManager(ClientConnectionManager &&) = delete;
    ClientConnectionManager &operator=(ClientConnectionManager &&) = delete;

    template <ServerOp Op> void sendServerOp(const typename Op::Payload &payload);

    void receiveServerOps(const ServerOpCallbacks &cbs);

  private:
    TClientTransport client_transport_;

    void receiveServerOp(const ServerOpCallbacks &cbs, ByteReader &reader);

    template <ServerOp Op> void dispatchServerOp(const ServerOpCallbacks &cbs, ByteReader &reader);
};

#endif
