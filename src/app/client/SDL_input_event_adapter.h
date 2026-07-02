#ifndef SDL_INPUT_EVENT_ADAPTER_H
#define SDL_INPUT_EVENT_ADAPTER_H

#include <SDL3/SDL.h>
#include <client/input_event.h>

class SDLInputEventAdapter
{
  public:
    SDLInputEventAdapter() = default;
    ~SDLInputEventAdapter() = default;
    SDLInputEventAdapter(const SDLInputEventAdapter &) = delete;
    SDLInputEventAdapter(SDLInputEventAdapter &&) = delete;
    SDLInputEventAdapter &operator=(const SDLInputEventAdapter &) = delete;
    SDLInputEventAdapter &operator=(SDLInputEventAdapter &&) = delete;

    void adapt(const SDL_Event &sdl_event)
    {
        switch (sdl_event.type)
        {
        case SDL_EVENT_KEY_DOWN:
            // TODO set up mapping from SDL key to generalised action (move north, etc.)
            // TODO drop generic typing if not helpful
            this->onKeyDown(sdl_event.key.key);
            break;
        }
    }

  private:
    void onKeyDown(SDL_Keycode key)
    {
        switch (key)
        {
        case SDLK_W:
            input<MoveNorthKeyDownEvent>({});
            break;
        case SDLK_D:
            input<MoveEastKeyDownEvent>({});
            break;
        case SDLK_S:
            input<MoveSouthKeyDownEvent>({});
            break;
        case SDLK_A:
            input<MoveWestKeyDownEvent>({});
            break;
        }
    }
};

#endif
