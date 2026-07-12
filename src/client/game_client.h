#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <array>
#include <client/client_connection_manager.h>
#include <client/input_event.h>
#include <core/game/loadout.h>
#include <core/util/direction.h>
#include <core/util/math.h>
#include <core/util/time.h>

template <typename TClientTransport> class GameClient
{
  public:
    struct Args
    {
        typename TClientTransport::Args client_transport_args;
        Loadout loadout;
    };

    struct View
    {
    };

    GameClient(const Args &args)
        : client_connection_manager_(args.client_transport_args), loadout_(args.loadout)
    {
    }
    ~GameClient() = default;

    GameClient(const GameClient &) = delete;
    GameClient &operator=(const GameClient &) = delete;

    GameClient(GameClient &&) = delete;
    GameClient &operator=(GameClient &&) = delete;

    void tick(milliseconds_t dt)
    {
        client_connection_manager_.tick();

        client_connection_manager_.receiveOpsFromServer({
            .onSpawnEntity =
                [this](const ServerSpawnEntityOpPayload &payload)
            {
                // TODO Handle spawn entity
            },
        });

        move(dt);

        client_connection_manager_.tick();
    }

    View view() const;

    void input(const KeyEvent &key_event)
    {
        const auto is_key_down = key_event.args.is_key_down;
        switch (key_event.type)
        {
        case InputEventType::MoveNorthKey:
            is_moving_in_cardinal_direction_[CardinalDirection::North] = is_key_down;
            break;
        case InputEventType::MoveEastKey:
            is_moving_in_cardinal_direction_[CardinalDirection::East] = is_key_down;
            break;
        case InputEventType::MoveSouthKey:
            is_moving_in_cardinal_direction_[CardinalDirection::South] = is_key_down;
            break;
        case InputEventType::MoveWestKey:
            is_moving_in_cardinal_direction_[CardinalDirection::West] = is_key_down;
            break;
        }
    }

  private:
    ClientConnectionManager<TClientTransport> client_connection_manager_;
    Loadout loadout_;

    std::array<bool, 4> is_moving_in_cardinal_direction_{false, false, false, false};

    double x_ = 0.0, y_ = 0.0;

    void move(milliseconds_t dt)
    {
        std::int8_t vertical_component = 0;
        if (is_moving_in_cardinal_direction_[CardinalDirection::North])
        {
            vertical_component -= 1;
        }
        if (is_moving_in_cardinal_direction_[CardinalDirection::South])
        {
            vertical_component += 1;
        }

        std::int8_t horizontal_component = 0;
        if (is_moving_in_cardinal_direction_[CardinalDirection::East])
        {
            horizontal_component += 1;
        }
        if (is_moving_in_cardinal_direction_[CardinalDirection::West])
        {
            horizontal_component -= 1;
        }

        if (vertical_component == 0 && horizontal_component == 0)
        {
            return;
        }

        double distance = loadout_.speed * static_cast<double>(dt);
        if (vertical_component != 0 && horizontal_component != 0)
        {
            distance *= M_HALF_SQRT_2;
        }

        x_ += static_cast<double>(horizontal_component) * distance;
        y_ += static_cast<double>(vertical_component) * distance;

        client_connection_manager_.template sendClientOpToServer<ClientMoveOp>({.x = x_, .y = y_});
    }
};

#endif
