#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <server/server_connection_manager.h>

template <typename TServerTransport> class GameServer
{
  public:
    GameServer(const typename TServerTransport::Args &server_transport_args)
        : server_connection_manager_(server_transport_args)
    {
    }
    ~GameServer() = default;

    GameServer(const GameServer &) = delete;
    GameServer &operator=(const GameServer &) = delete;

    GameServer(GameServer &&) = delete;
    GameServer &operator=(GameServer &&) = delete;

    [[nodiscard]] ServerConnectionId connectionId() const noexcept
    {
        return server_connection_manager_.connectionId();
    }

    void tick()
    {
        // TODO Game server tick
    }

  private:
    ServerConnectionManager<TServerTransport> server_connection_manager_;
};

#endif
