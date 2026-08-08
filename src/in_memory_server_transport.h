#ifndef IN_MEMORY_SERVER_TRANSPORT_H
#define IN_MEMORY_SERVER_TRANSPORT_H

#include "transport.h"
#include <map>
#include <memory>

class InMemoryNetwork;

class InMemoryServerTransport : public ServerTransport
{
  public:
    struct Args
    {
        std::shared_ptr<InMemoryNetwork> network;
    };

    InMemoryServerTransport(const Args &args, ServerTransportCallbacks callbacks);

    ~InMemoryServerTransport() override;

    InMemoryServerTransport(const InMemoryServerTransport &) = delete;
    InMemoryServerTransport &operator=(const InMemoryServerTransport &) = delete;

    InMemoryServerTransport(InMemoryServerTransport &&) = delete;
    InMemoryServerTransport &operator=(InMemoryServerTransport &&) = delete;

    void send(ClientConnectionId client_id, std::span<const std::byte> data) override;
    std::vector<std::byte> receive(ClientConnectionId client_id) override;
    void disconnectClient(ClientConnectionId client_id) override;
    void close() override;

    void tick();

    [[nodiscard]] ServerConnectionId connectionId() const noexcept { return connection_id_; }

  private:
    const std::shared_ptr<InMemoryNetwork> network_;
    const ServerConnectionId connection_id_;
    const ServerTransportCallbacks callbacks_;
    bool is_closed_ = false;
    std::map<ClientConnectionId, std::vector<std::byte>> send_buffers_;
    std::map<ClientConnectionId, std::vector<std::byte>> receive_buffers_;

    void onClientDisconnected(ClientConnectionId client_id);
    void connectClient(ClientConnectionId client_id);
    void closeImpl();
};

#endif
