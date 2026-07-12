#ifndef CLIENT_APP_H
#define CLIENT_APP_H

#include "single_player_session.h"

class ClientApp
{
  public:
    using View = SinglePlayerSession::View;

    ClientApp() = default;
    ~ClientApp() = default;
    ClientApp(const ClientApp &) = delete;
    ClientApp(ClientApp &&) = delete;
    ClientApp &operator=(const ClientApp &) = delete;
    ClientApp &operator=(ClientApp &&) = delete;

    [[nodiscard]] View view() const { return single_player_session_.view(); }

    template <typename InputEvent> void input(const InputEvent &event)
    {
        single_player_session_.input<InputEvent>(event);
    }

  private:
    SinglePlayerSession single_player_session_{
        SinglePlayerSession::Args{.loadout = Loadout{.id = LoadoutId::Warper, .speed = 0.1}}};

    void tick(milliseconds_t dt) { single_player_session_.tick(dt); }
};

#endif
