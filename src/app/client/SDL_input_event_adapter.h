#ifndef SDL_INPUT_EVENT_ADAPTER_H
#define SDL_INPUT_EVENT_ADAPTER_H

#include "SDL3/SDL_keycode.h"
#include <SDL3/SDL.h>
#include <client/input_event.h>
#include <unordered_map>

class SDLInputEventAdapter
{
  public:
    SDLInputEventAdapter() = default;
    ~SDLInputEventAdapter() = default;
    SDLInputEventAdapter(const SDLInputEventAdapter &) = delete;
    SDLInputEventAdapter(SDLInputEventAdapter &&) = delete;
    SDLInputEventAdapter &operator=(const SDLInputEventAdapter &) = delete;
    SDLInputEventAdapter &operator=(SDLInputEventAdapter &&) = delete;

    template <typename InputEventHandler>
    void adapt(const SDL_Event &sdl_event, const InputEventHandler &handler)
    {
        switch (sdl_event.type)
        {
        case SDL_EVENT_KEY_DOWN:
            this->onKeyEvent(sdl_event.key.key, true, handler);
            break;
        case SDL_EVENT_KEY_UP:
            this->onKeyEvent(sdl_event.key.key, false, handler);
            break;
        }
    }

  private:
    std::unordered_map<SDL_Keycode, InputEventType> keycode_to_input_event_type_{
        {SDLK_W, InputEventType::MoveNorthKey},
        {SDLK_D, InputEventType::MoveEastKey},
        {SDLK_S, InputEventType::MoveSouthKey},
        {SDLK_A, InputEventType::MoveWestKey},
    };

    template <typename InputEventHandler>
    void onKeyEvent(SDL_Keycode key, bool is_key_down, const InputEventHandler &handler)
    {
        auto it = keycode_to_input_event_type_.find(key);
        if (it == keycode_to_input_event_type_.end())
        {
            return;
        }

        InputEventType type = it->second;
        KeyEvent::Args args{.is_key_down = is_key_down};
        KeyEvent event{.type = type, .args = args};

        handler.input(event);
    }
};

#endif
