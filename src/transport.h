#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <cstddef>
#include <functional>
#include <span>
#include <vector>

using ClientConnectionId = std::size_t;
using ServerConnectionId = std::size_t;

class ClientTransport
{
  public:
    ClientTransport() = default;
    virtual ~ClientTransport() = default;

    ClientTransport(const ClientTransport &) = delete;
    ClientTransport &operator=(const ClientTransport &) = delete;

    ClientTransport(ClientTransport &&) = delete;
    ClientTransport &operator=(ClientTransport &&) = delete;

    virtual void send(std::span<const std::byte>) = 0;
    virtual std::vector<std::byte> receive() = 0;
    virtual void disconnect() = 0;
};

class ServerTransport
{
  public:
    ServerTransport() = default;
    virtual ~ServerTransport() = default;

    ServerTransport(const ServerTransport &) = delete;
    ServerTransport &operator=(const ServerTransport &) = delete;

    ServerTransport(ServerTransport &&) = delete;
    ServerTransport &operator=(ServerTransport &&) = delete;

    virtual void send(ClientConnectionId, std::span<const std::byte>) = 0;
    virtual std::vector<std::byte> receive(ClientConnectionId) = 0;
    virtual void disconnectClient(ClientConnectionId) = 0;
    virtual void close() = 0;
};

struct ClientTransportCallbacks
{
    std::function<void()> on_disconnected;
};

struct ServerTransportCallbacks
{
    std::function<void(ClientConnectionId)> on_client_connected;
    std::function<void(ClientConnectionId)> on_client_disconnected;
};

#endif
