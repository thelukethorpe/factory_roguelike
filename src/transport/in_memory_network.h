#ifndef IN_MEMORY_NETWORK_H
#define IN_MEMORY_NETWORK_H

#include "in_memory_client_transport.h"
#include "in_memory_server_transport.h"

struct InMemoryNetworkBuffers
{
    std::vector<std::byte> to_client_buffer;
    std::vector<std::byte> to_server_buffer;
};

class InMemoryNetwork
{
  private:
    ServerConnectionId next_server_id_ = 0;
    ClientConnectionId next_client_id_ = 0;

    std::map<ServerConnectionId, std::map<ClientConnectionId, InMemoryNetworkBuffers>>
        server_to_client_to_buffers_map_;

    friend InMemoryClientTransport;
    friend InMemoryServerTransport;

    bool tryConnectClientToServer(const ClientServerPair &client_server_pair);
    bool tryDisconnectClientFromServer(const ClientServerPair &client_server_pair);
    InMemoryNetworkBuffers *
    getBuffersForClientServerPair(const ClientServerPair &client_server_pair);

    void openServer(ServerConnectionId server_id);
    void closeServer(ServerConnectionId server_id);
    std::vector<ClientConnectionId> getClientsConnectedToServer(ServerConnectionId server_id);
};

#endif
