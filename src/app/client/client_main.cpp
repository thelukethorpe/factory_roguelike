#include "single_player_session.h"
#include <core/util/log.h>
#include <core/util/time.h>

int main()
{
    LOG_INFO("Starting client application...");
    Loadout loadout{
        .id = LoadoutId::Warper,
        .speed = 0.1,
    };
    auto previous_now = now();
    SinglePlayerSession session{SinglePlayerSession::Args{.loadout = loadout}};
    for (int i = 0; i < 10; ++i)
    {
        const auto current_now = now();
        session.tick(current_now - previous_now);
        previous_now = current_now;
    }
    return 0;
}
