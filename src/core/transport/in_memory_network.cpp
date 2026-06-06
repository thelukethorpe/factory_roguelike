#include "in_memory_network.h"

bool InMemoryNetwork::tryConnectClientToServer(const ClientServerPair &client_server_pair)
{
    const auto server_it = server_to_client_to_buffers_map_.find(client_server_pair.server_id);
    if (server_it == server_to_client_to_buffers_map_.end())
    {
        return false;
    }

    auto &client_to_buffers_map = server_it->second;
    client_to_buffers_map.emplace(client_server_pair.client_id, InMemoryNetworkBuffers{});
    return true;
}

bool InMemoryNetwork::tryDisconnectClientFromServer(const ClientServerPair &client_server_pair)
{
    const auto server_it = server_to_client_to_buffers_map_.find(client_server_pair.server_id);
    if (server_it == server_to_client_to_buffers_map_.end())
    {
        return false;
    }

    auto &client_to_buffers_map = server_it->second;
    const auto num_erased = client_to_buffers_map.erase(client_server_pair.client_id);
    return num_erased > 0;
}

InMemoryNetworkBuffers *
InMemoryNetwork::getBuffersForClientServerPair(const ClientServerPair &client_server_pair)
{
    const auto server_it = server_to_client_to_buffers_map_.find(client_server_pair.server_id);
    if (server_it == server_to_client_to_buffers_map_.end())
    {
        return nullptr;
    }

    auto &client_to_buffers_map = server_it->second;
    const auto buffers_it = client_to_buffers_map.find(client_server_pair.client_id);
    if (buffers_it == client_to_buffers_map.end())
    {
        return nullptr;
    }

    return &buffers_it->second;
}

void InMemoryNetwork::openServer(const ServerConnectionId server_id)
{
    server_to_client_to_buffers_map_.emplace(
        server_id, std::map<ClientConnectionId, InMemoryNetworkBuffers>{});
}

void InMemoryNetwork::closeServer(const ServerConnectionId server_id)
{
    server_to_client_to_buffers_map_.erase(server_id);
}

std::vector<ClientConnectionId>
InMemoryNetwork::getClientsConnectedToServer(ServerConnectionId server_id)
{
    const auto server_it = server_to_client_to_buffers_map_.find(server_id);
    if (server_it == server_to_client_to_buffers_map_.end())
    {
        return {};
    }

    auto &client_to_buffers_map = server_it->second;
    std::vector<ClientConnectionId> client_ids;
    client_ids.reserve(client_to_buffers_map.size());
    for (const auto &[client_id, buffers] : client_to_buffers_map)
    {
        client_ids.push_back(client_id);
    }

    return client_ids;
}
