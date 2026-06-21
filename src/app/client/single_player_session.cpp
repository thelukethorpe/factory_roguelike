#include "single_player_session.h"

SinglePlayerSession::SinglePlayerSession(const Args &args)
    : network_(std::make_shared<InMemoryNetwork>()),
      game_server_(InMemoryServerTransport::Args{.network = network_}),
      game_client_({
          .client_transport_args =
              InMemoryClientTransport::Args{.network = network_,
                                            .server_id = game_server_.connectionId()},
          .loadout = args.loadout,
      })
{
}

void SinglePlayerSession::tick(milliseconds_t dt)
{
    game_client_.tick(dt);
    game_server_.tick();
}
