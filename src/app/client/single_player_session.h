#ifndef SINGLE_PLAYER_SESSION_H
#define SINGLE_PLAYER_SESSION_H

#include <client/game_client.h>
#include <core/game/loadout.h>
#include <core/transport/in_memory_network.h>
#include <core/util/log.h>
#include <core/util/time.h>
#include <server/game_server.h>

using SinglePlayerGameServer = GameServer<InMemoryServerTransport>;
using SinglePlayerGameClient = GameClient<InMemoryClientTransport>;

class SinglePlayerSession
{
  public:
    struct Args
    {
        Loadout loadout;
    };

    using View = SinglePlayerGameClient::View;

    SinglePlayerSession(const Args &args);
    ~SinglePlayerSession() = default;

    SinglePlayerSession(const SinglePlayerSession &) = delete;
    SinglePlayerSession &operator=(const SinglePlayerSession &) = delete;

    SinglePlayerSession(SinglePlayerSession &&) = delete;
    SinglePlayerSession &operator=(SinglePlayerSession &&) = delete;

    void tick(milliseconds_t dt);

    [[nodiscard]] View view() const { return game_client_.view(); }

    template <typename InputEvent> void input(const typename InputEvent::Args &args)
    {
        game_client_.input<InputEvent>(args);
    }

  private:
    std::shared_ptr<InMemoryNetwork> network_;
    SinglePlayerGameServer game_server_;
    SinglePlayerGameClient game_client_;

    static void onClientMove(const ClientMoveOpPayload &payload)
    {
        LOG_INFO("Server received move from client: x={}, y={}", payload.x, payload.y);
    }
};

#endif
