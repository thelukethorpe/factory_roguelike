#include "in_memory_client_transport.h"
#include "in_memory_network.h"

InMemoryClientTransport::InMemoryClientTransport(const Args &args,
                                                 ClientTransportCallbacks callbacks)
    : network_(args.network), client_server_pair_{.client_id = args.network->next_client_id_++,
                                                  .server_id = args.server_id},
      callbacks_(std::move(callbacks))
{
    const auto didConnect = network_->tryConnectClientToServer(client_server_pair_);
    if (!didConnect)
    {
        this->disconnectImpl();
    }
};

InMemoryClientTransport::~InMemoryClientTransport() { this->disconnectImpl(); }

void InMemoryClientTransport::send(std::span<const std::byte> data)
{
    send_buffer_.insert(send_buffer_.end(), data.begin(), data.end());
}

std::vector<std::byte> InMemoryClientTransport::receive()
{
    const auto result = std::move(receive_buffer_);
    receive_buffer_.clear();
    return result;
}

void InMemoryClientTransport::disconnect() { this->disconnectImpl(); }

void InMemoryClientTransport::tick()
{
    auto *const buffers = network_->getBuffersForClientServerPair(client_server_pair_);
    if (buffers == nullptr)
    {
        this->disconnect();
        return;
    }

    auto &to_server_buffer = buffers->to_server_buffer;
    to_server_buffer.insert(to_server_buffer.end(), send_buffer_.begin(), send_buffer_.end());
    send_buffer_.clear();

    auto &to_client_buffer = buffers->to_client_buffer;
    receive_buffer_.insert(receive_buffer_.end(), to_client_buffer.begin(), to_client_buffer.end());
    to_client_buffer.clear();
}

void InMemoryClientTransport::disconnectImpl()
{
    if (!is_connected_)
    {
        return;
    }

    is_connected_ = false;
    network_->tryDisconnectClientFromServer(client_server_pair_);

    if (callbacks_.on_disconnected)
    {
        callbacks_.on_disconnected();
    }
}
