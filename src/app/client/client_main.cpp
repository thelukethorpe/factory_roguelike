#include "SDL/SDL_asset_registrar.h"
#include "SDL/SDL_input_event_adapter.h"
#include "client_app.h"
#include <core/util/log.h>
#include <core/util/time.h>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

struct ClientAppState
{
    SDLAssetRegistrar asset_registrar;
    SDLInputEventAdapter input_event_adapter;
    ClientApp client_app;
};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Textures", "1.0", "com.example.renderer-textures");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/textures", WINDOW_WIDTH, WINDOW_HEIGHT,
                                     SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);

    auto *mem = SDL_calloc(1, sizeof(ClientAppState));
    if (mem == nullptr)
    {
        return SDL_APP_FAILURE;
    }

    auto *client_app_state =
        new (mem) ClientAppState{.asset_registrar = SDLAssetRegistrar{renderer},
                                 .input_event_adapter = SDLInputEventAdapter{},
                                 .client_app = ClientApp{}};

    *appstate = client_app_state;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    auto *client_app_state = static_cast<ClientAppState *>(appstate);
    auto &input_event_adapter = client_app_state->input_event_adapter;
    auto &client_app = client_app_state->client_app;

    // TODO method name
    input_event_adapter.adapt(*event, client_app);

    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    auto *client_app_state = static_cast<ClientAppState *>(appstate);
    auto &asset_registrar = client_app_state->asset_registrar;
    auto &client_app = client_app_state->client_app;

    auto view = client_app.view();

    // TODO logic tick should happen in separate thread - 2 view buffers should be maintained, and
    // the view buffer should be swapped after the logic tick is complete. The rendering thread
    // should always render the current view buffer, and the logic thread should always update the
    // next view buffer. Each ECS comp should have a "tick" and an "updateView" / "view" method
    // Either that, or there is an ECS comp that runs last that updates the view buffer

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_FRect dst_rect;

    // TODO loop over view
    const auto asset = asset_registrar.getPlayerTexture(LoadoutId::Warper);
    // dst_rect.x = static_cast<float>(view.player_x);
    // dst_rect.y = static_cast<float>(view.player_y);
    dst_rect.w = static_cast<float>(asset.width);
    dst_rect.h = static_cast<float>(asset.height);
    SDL_RenderTexture(renderer, asset.sdl_texture, nullptr, &dst_rect);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if (appstate == nullptr)
    {
        return;
    }

    auto *client_app_state = static_cast<ClientAppState *>(appstate);
    client_app_state->~ClientAppState();
    SDL_free(client_app_state);
}

// int main()
// {
//     LOG_INFO("Starting client application...");
//     Loadout loadout{
//         .id = LoadoutId::Warper,
//         .speed = 0.1,
//     };
//     auto previous_now = now();
//     SinglePlayerSession session{SinglePlayerSession::Args{.loadout = loadout}};
//     for (int i = 0; i < 10; ++i)
//     {
//         const auto current_now = now();
//         session.tick(current_now - previous_now);
//         previous_now = current_now;
//     }
//     return 0;
// }
