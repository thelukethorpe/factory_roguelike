#include "in_memory_server_transport.h"
#include "in_memory_network.h"

InMemoryServerTransport::InMemoryServerTransport(const Args &args,
                                                 ServerTransportCallbacks callbacks)
    : network_(args.network), connection_id_(network_->next_server_id_++),
      callbacks_(std::move(callbacks))
{
    network_->openServer(connection_id_);
}

InMemoryServerTransport::~InMemoryServerTransport() { this->closeImpl(); }

void InMemoryServerTransport::send(ClientConnectionId client_id, std::span<const std::byte> data)
{
    const auto send_buffer_it = send_buffers_.find(client_id);
    if (send_buffer_it == send_buffers_.end())
    {
        return;
    }

    auto &send_buffer = send_buffer_it->second;
    send_buffer.insert(send_buffer.end(), data.begin(), data.end());
}

std::vector<std::byte> InMemoryServerTransport::receive(ClientConnectionId client_id)
{
    const auto receive_buffer_it = receive_buffers_.find(client_id);
    if (receive_buffer_it == receive_buffers_.end())
    {
        return {};
    }

    auto &receive_buffer = receive_buffer_it->second;
    const auto result = std::move(receive_buffer);
    receive_buffer.clear();
    return result;
}

void InMemoryServerTransport::disconnectClient(ClientConnectionId client_id)
{
    const auto didDisconnect = network_->tryDisconnectClientFromServer(
        ClientServerPair{.client_id = client_id, .server_id = connection_id_});
    if (didDisconnect)
    {
        this->onClientDisconnected(client_id);
    }
}

void InMemoryServerTransport::close() { this->closeImpl(); }

void InMemoryServerTransport::tick()
{
    if (is_closed_)
    {
        return;
    }

    const auto connected_client_ids = network_->getClientsConnectedToServer(connection_id_);
    for (const auto client_id : connected_client_ids)
    {
        if (!send_buffers_.contains(client_id))
        {
            this->connectClient(client_id);
        }
    }

    for (auto &[client_id, send_buffer] : send_buffers_)
    {
        const ClientServerPair client_server_pair{.client_id = client_id,
                                                  .server_id = connection_id_};
        auto *const buffers = network_->getBuffersForClientServerPair(client_server_pair);
        if (buffers == nullptr)
        {
            this->onClientDisconnected(client_id);
            continue;
        }

        auto &to_client_buffer = buffers->to_client_buffer;
        to_client_buffer.insert(to_client_buffer.end(), send_buffer.begin(), send_buffer.end());
        send_buffer.clear();

        auto &to_server_buffer = buffers->to_server_buffer;
        auto &receive_buffer = receive_buffers_[client_id];
        receive_buffer.insert(receive_buffer.end(), to_server_buffer.begin(),
                              to_server_buffer.end());
        to_server_buffer.clear();
    }
}

void InMemoryServerTransport::onClientDisconnected(ClientConnectionId client_id)
{
    send_buffers_.erase(client_id);
    receive_buffers_.erase(client_id);

    if (callbacks_.on_client_disconnected)
    {
        callbacks_.on_client_disconnected(client_id);
    }
}

void InMemoryServerTransport::connectClient(ClientConnectionId client_id)
{
    send_buffers_.emplace(client_id, std::vector<std::byte>{});
    receive_buffers_.emplace(client_id, std::vector<std::byte>{});

    if (callbacks_.on_client_connected)
    {
        callbacks_.on_client_connected(client_id);
    }
}

void InMemoryServerTransport::closeImpl()
{
    if (is_closed_)
    {
        return;
    }

    is_closed_ = true;

    // Copy client IDs into a new vector to avoid modifying the map while iterating over it.
    std::vector<ClientConnectionId> client_ids;
    client_ids.reserve(send_buffers_.size());
    for (const auto &[client_id, send_buffer] : send_buffers_)
    {
        client_ids.push_back(client_id);
    }

    for (const auto client_id : client_ids)
    {
        this->disconnectClient(client_id);
    }

    network_->closeServer(connection_id_);
}
