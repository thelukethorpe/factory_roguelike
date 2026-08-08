#include <in_memory_client_transport.h>
#include <in_memory_network.h>
#include <in_memory_server_transport.h>
#include <cstddef>
#include <doctest/doctest.h>
#include <memory>
#include <set>
#include <vector>

TEST_CASE("In-memory transport client-server communication")
{
    std::shared_ptr<InMemoryNetwork> network = std::make_shared<InMemoryNetwork>();
    InMemoryServerTransport server({.network = network}, ServerTransportCallbacks{});

    InMemoryClientTransport client_1({.network = network, .server_id = server.connectionId()},
                                     ClientTransportCallbacks{});
    InMemoryClientTransport client_2({.network = network, .server_id = server.connectionId()},
                                     ClientTransportCallbacks{});

    server.tick();

    SUBCASE("Clients receive messages sent by server")
    {
        const std::vector<std::byte> data_1 = {std::byte{0x01}, std::byte{0x02}, std::byte{0x03}};
        server.send(client_1.connectionId(), data_1);

        const std::vector<std::byte> data_2 = {std::byte{0x04}, std::byte{0x05}, std::byte{0x06}};
        server.send(client_2.connectionId(), data_2);

        server.tick();

        client_1.tick();
        client_2.tick();

        CHECK(client_1.receive() == data_1);
        CHECK(client_2.receive() == data_2);
        CHECK(client_1.receive().empty());
        CHECK(client_2.receive().empty());

        // Send a second round of messages to verify that buffers are cleared correctly after each
        // tick.
        const std::vector<std::byte> data_3 = {std::byte{0x11}, std::byte{0x12}, std::byte{0x13}};
        server.send(client_1.connectionId(), data_3);

        const std::vector<std::byte> data_4 = {std::byte{0x14}, std::byte{0x15}, std::byte{0x16}};
        server.send(client_2.connectionId(), data_4);

        server.tick();

        client_1.tick();
        client_2.tick();

        CHECK(client_1.receive() == data_3);
        CHECK(client_2.receive() == data_4);
        CHECK(client_1.receive().empty());
        CHECK(client_2.receive().empty());
    }

    SUBCASE("Clients receive all data enqueued by server")
    {
        const std::vector<std::byte> data_1 = {std::byte{0x01}, std::byte{0x02}, std::byte{0x03}};
        const std::vector<std::byte> data_2 = {std::byte{0x04}, std::byte{0x05}, std::byte{0x06}};
        server.send(client_1.connectionId(), data_1);
        server.send(client_1.connectionId(), data_2);

        server.tick();

        client_1.tick();

        std::vector<std::byte> combined_data;
        combined_data.insert(combined_data.end(), data_1.begin(), data_1.end());
        combined_data.insert(combined_data.end(), data_2.begin(), data_2.end());

        CHECK(client_1.receive() == combined_data);
        CHECK(client_1.receive().empty());
    }

    SUBCASE("Server receives messages sent by clients")
    {
        const std::vector<std::byte> data_1 = {std::byte{0x01}, std::byte{0x02}, std::byte{0x03}};
        client_1.send(data_1);

        const std::vector<std::byte> data_2 = {std::byte{0x04}, std::byte{0x05}, std::byte{0x06}};
        client_2.send(data_2);

        client_1.tick();
        client_2.tick();

        server.tick();

        CHECK(server.receive(client_1.connectionId()) == data_1);
        CHECK(server.receive(client_2.connectionId()) == data_2);

        CHECK(server.receive(client_1.connectionId()).empty());
        CHECK(server.receive(client_2.connectionId()).empty());

        // Send a second round of messages to verify that buffers are cleared correctly after each
        // tick.
        const std::vector<std::byte> data_3 = {std::byte{0x11}, std::byte{0x12}, std::byte{0x13}};
        client_1.send(data_3);
        client_1.tick();

        const std::vector<std::byte> data_4 = {std::byte{0x14}, std::byte{0x15}, std::byte{0x16}};
        client_2.send(data_4);
        client_2.tick();

        server.tick();
        CHECK(server.receive(client_1.connectionId()) == data_3);
        CHECK(server.receive(client_2.connectionId()) == data_4);

        CHECK(server.receive(client_1.connectionId()).empty());
        CHECK(server.receive(client_2.connectionId()).empty());
    }

    SUBCASE("Bi-directional communication")
    {
        const std::vector<std::byte> data_from_client_1 = {std::byte{0x01}, std::byte{0x02},
                                                           std::byte{0x03}};
        client_1.send(data_from_client_1);

        const std::vector<std::byte> data_from_client_2 = {std::byte{0x04}, std::byte{0x05},
                                                           std::byte{0x06}};
        client_2.send(data_from_client_2);

        client_1.tick();
        client_2.tick();

        const std::vector<std::byte> data_from_server_1 = {std::byte{0x07}, std::byte{0x08},
                                                           std::byte{0x09}};
        server.send(client_1.connectionId(), data_from_server_1);

        const std::vector<std::byte> data_from_server_2 = {std::byte{0x0A}, std::byte{0x0B},
                                                           std::byte{0x0C}};
        server.send(client_2.connectionId(), data_from_server_2);

        server.tick();

        client_1.tick();
        client_2.tick();

        CHECK(client_1.receive() == data_from_server_1);
        CHECK(client_2.receive() == data_from_server_2);
        CHECK(server.receive(client_1.connectionId()) == data_from_client_1);
        CHECK(server.receive(client_2.connectionId()) == data_from_client_2);

        CHECK(client_1.receive().empty());
        CHECK(client_2.receive().empty());
        CHECK(server.receive(client_1.connectionId()).empty());
        CHECK(server.receive(client_2.connectionId()).empty());

        // Send a second round of messages to verify that buffers are cleared correctly after each
        // tick.
        const std::vector<std::byte> data_from_client3 = {std::byte{0x11}, std::byte{0x12},
                                                          std::byte{0x13}};
        client_1.send(data_from_client3);

        const std::vector<std::byte> data_from_client4 = {std::byte{0x14}, std::byte{0x15},
                                                          std::byte{0x16}};
        client_2.send(data_from_client4);

        client_1.tick();
        client_2.tick();

        const std::vector<std::byte> data_from_server3 = {std::byte{0x17}, std::byte{0x18},
                                                          std::byte{0x19}};
        server.send(client_1.connectionId(), data_from_server3);

        const std::vector<std::byte> data_from_server4 = {std::byte{0x1A}, std::byte{0x1B},
                                                          std::byte{0x1C}};
        server.send(client_2.connectionId(), data_from_server4);

        server.tick();

        client_1.tick();
        client_2.tick();

        CHECK(client_1.receive() == data_from_server3);
        CHECK(client_2.receive() == data_from_server4);
        CHECK(server.receive(client_1.connectionId()) == data_from_client3);
        CHECK(server.receive(client_2.connectionId()) == data_from_client4);

        CHECK(client_1.receive().empty());
        CHECK(client_2.receive().empty());
        CHECK(server.receive(client_1.connectionId()).empty());
        CHECK(server.receive(client_2.connectionId()).empty());
    }
}

TEST_CASE("In-memory server transport callbacks")
{
    std::vector<ClientConnectionId> connection_events;
    std::vector<ClientConnectionId> disconnection_events;

    std::shared_ptr<InMemoryNetwork> network = std::make_shared<InMemoryNetwork>();
    InMemoryServerTransport server(
        {.network = network},
        ServerTransportCallbacks{
            .on_client_connected = [&connection_events](ClientConnectionId client_id)
            { connection_events.push_back(client_id); },
            .on_client_disconnected = [&disconnection_events](ClientConnectionId client_id)
            { disconnection_events.push_back(client_id); },
        });

    const auto as_set = [](const std::vector<ClientConnectionId> &ids)
    { return std::set<ClientConnectionId>(ids.begin(), ids.end()); };

    SUBCASE("Connect callback fires for newly connected clients only")
    {
        InMemoryClientTransport client_1({.network = network, .server_id = server.connectionId()},
                                         ClientTransportCallbacks{});
        server.tick();
        CHECK(connection_events == std::vector<ClientConnectionId>{client_1.connectionId()});
        CHECK(disconnection_events.empty());

        // No new clients since last tick.
        server.tick();
        CHECK(connection_events == std::vector<ClientConnectionId>{client_1.connectionId()});
        CHECK(disconnection_events.empty());

        InMemoryClientTransport client_2({.network = network, .server_id = server.connectionId()},
                                         ClientTransportCallbacks{});
        server.tick();
        CHECK(connection_events ==
              std::vector<ClientConnectionId>{client_1.connectionId(), client_2.connectionId()});
        CHECK(disconnection_events.empty());
    }

    SUBCASE("Disconnect callback fires once when client disconnects itself")
    {
        InMemoryClientTransport client({.network = network, .server_id = server.connectionId()},
                                       ClientTransportCallbacks{});
        server.tick();

        CHECK(disconnection_events.empty());

        client.disconnect();
        server.tick();

        CHECK(disconnection_events == std::vector<ClientConnectionId>{client.connectionId()});

        // Repeated disconnect/tick should not emit duplicates.
        client.disconnect();
        server.tick();
        CHECK(disconnection_events == std::vector<ClientConnectionId>{client.connectionId()});
    }

    SUBCASE("Disconnect callback fires once when server disconnects a client")
    {
        InMemoryClientTransport client({.network = network, .server_id = server.connectionId()},
                                       ClientTransportCallbacks{});
        server.tick();

        server.disconnectClient(client.connectionId());

        CHECK(disconnection_events == std::vector<ClientConnectionId>{client.connectionId()});

        // Repeated server disconnect should be a no-op.
        server.disconnectClient(client.connectionId());
        CHECK(disconnection_events == std::vector<ClientConnectionId>{client.connectionId()});

        // Client notices disconnection, but server should not emit again.
        client.tick();
        server.tick();
        CHECK(disconnection_events == std::vector<ClientConnectionId>{client.connectionId()});
    }

    SUBCASE("Close emits disconnect callback for all connected clients exactly once")
    {
        InMemoryClientTransport client_1({.network = network, .server_id = server.connectionId()},
                                         ClientTransportCallbacks{});
        InMemoryClientTransport client_2({.network = network, .server_id = server.connectionId()},
                                         ClientTransportCallbacks{});
        server.tick();

        connection_events.clear();
        disconnection_events.clear();

        server.close();

        CHECK(as_set(disconnection_events) ==
              std::set<ClientConnectionId>{client_1.connectionId(), client_2.connectionId()});
        CHECK(disconnection_events.size() == 2);

        // Repeated close should not emit duplicates.
        server.close();
        CHECK(disconnection_events.size() == 2);
    }
}

TEST_CASE("In-memory client transport callbacks")
{
    std::shared_ptr<InMemoryNetwork> network = std::make_shared<InMemoryNetwork>();
    InMemoryServerTransport server({.network = network}, ServerTransportCallbacks{});

    SUBCASE("Client disconnect callback is called on explicit disconnect exactly once")
    {
        int disconnect_count = 0;
        {
            InMemoryClientTransport client(
                {.network = network, .server_id = server.connectionId()},
                ClientTransportCallbacks{
                    .on_disconnected = [&disconnect_count]() { disconnect_count++; },
                });

            server.tick();
            CHECK(disconnect_count == 0);

            client.disconnect();
            CHECK(disconnect_count == 1);

            // Repeated disconnect should be a no-op.
            client.disconnect();
            CHECK(disconnect_count == 1);
        }

        // Destructor should not trigger callback again after prior disconnect.
        CHECK(disconnect_count == 1);
    }

    SUBCASE("Client disconnect callback is called when server disconnects that client")
    {
        int disconnect_count = 0;
        InMemoryClientTransport client(
            {.network = network, .server_id = server.connectionId()},
            ClientTransportCallbacks{
                .on_disconnected = [&disconnect_count]() { disconnect_count++; },
            });

        server.tick();
        CHECK(disconnect_count == 0);

        server.disconnectClient(client.connectionId());
        client.tick();

        CHECK(disconnect_count == 1);

        // Repeated ticks after disconnect should not re-fire callback.
        client.tick();
        CHECK(disconnect_count == 1);
    }

    SUBCASE("Client disconnect callback is called when server closes")
    {
        int disconnect_count = 0;
        InMemoryClientTransport client(
            {.network = network, .server_id = server.connectionId()},
            ClientTransportCallbacks{
                .on_disconnected = [&disconnect_count]() { disconnect_count++; },
            });

        server.tick();
        CHECK(disconnect_count == 0);

        server.close();
        client.tick();

        CHECK(disconnect_count == 1);
    }

    SUBCASE("Client disconnect callback is called if initial connect fails")
    {
        int disconnect_count = 0;

        // No server is open at this ID, so constructor should fail to connect.
        const ServerConnectionId invalid_server_id = server.connectionId() + 1;
        InMemoryClientTransport client(
            {.network = network, .server_id = invalid_server_id},
            ClientTransportCallbacks{
                .on_disconnected = [&disconnect_count]() { disconnect_count++; },
            });

        CHECK(disconnect_count == 1);

        // Tick and explicit disconnect should not trigger additional callbacks.
        client.tick();
        client.disconnect();
        CHECK(disconnect_count == 1);
    }
}
