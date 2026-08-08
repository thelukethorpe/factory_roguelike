#ifndef SINGLE_PLAYER_SESSION_H
#define SINGLE_PLAYER_SESSION_H

#include <client/game_client.h>
#include <core/game/loadout.h>
#include <core/util/log.h>
#include <core/util/time.h>
#include <cpp_transport/in_memory_network.h>
#include <server/game_server.h>

class SinglePlayerSession
{
  public:
    struct Args
    {
        Loadout loadout;
    };

    SinglePlayerSession(const Args &args);
    ~SinglePlayerSession() = default;

    SinglePlayerSession(const SinglePlayerSession &) = delete;
    SinglePlayerSession &operator=(const SinglePlayerSession &) = delete;

    SinglePlayerSession(SinglePlayerSession &&) = delete;
    SinglePlayerSession &operator=(SinglePlayerSession &&) = delete;

    void tick(milliseconds_t dt);

  private:
    std::shared_ptr<InMemoryNetwork> network_;
    GameServer<InMemoryServerTransport> game_server_;
    GameClient<InMemoryClientTransport> game_client_;

    static void onClientMove(const ClientMoveOpPayload &payload)
    {
        LOG_INFO("Server received move from client: x={}, y={}", payload.x, payload.y);
    }
};

#endif
