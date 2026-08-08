#ifndef IN_MEMORY_CLIENT_TRANSPORT_H
#define IN_MEMORY_CLIENT_TRANSPORT_H

#include "transport.h"
#include <memory>

class InMemoryNetwork;

struct ClientServerPair
{
    ClientConnectionId client_id;
    ServerConnectionId server_id;
};

class InMemoryClientTransport : public ClientTransport
{
  public:
    struct Args
    {
        std::shared_ptr<InMemoryNetwork> network;
        ServerConnectionId server_id;
    };

    InMemoryClientTransport(const Args &args, ClientTransportCallbacks callbacks);

    ~InMemoryClientTransport() override;

    InMemoryClientTransport(const InMemoryClientTransport &) = delete;
    InMemoryClientTransport &operator=(const InMemoryClientTransport &) = delete;

    InMemoryClientTransport(InMemoryClientTransport &&) = delete;
    InMemoryClientTransport &operator=(InMemoryClientTransport &&) = delete;

    void send(std::span<const std::byte> data) override;
    std::vector<std::byte> receive() override;
    void disconnect() override;

    void tick();

    [[nodiscard]] ClientConnectionId connectionId() const noexcept
    {
        return client_server_pair_.client_id;
    }

  private:
    const std::shared_ptr<InMemoryNetwork> network_;
    const ClientServerPair client_server_pair_;
    const ClientTransportCallbacks callbacks_;
    bool is_connected_ = true;
    std::vector<std::byte> send_buffer_;
    std::vector<std::byte> receive_buffer_;

    void disconnectImpl();
};

#endif
