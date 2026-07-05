#include "SDL/SDL_input_event_adapter.h"
#include "client_app.h"
#include <core/util/log.h>
#include <core/util/time.h>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;
// static SDL_Texture *texture = nullptr;
// static int texture_width = 0;
// static int texture_height = 0;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

struct ClientAppState
{
    SDLInputEventAdapter input_event_adapter;
    ClientApp client_app;
};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    auto *mem = SDL_calloc(1, sizeof(ClientAppState));
    if (mem == nullptr)
    {
        return SDL_APP_FAILURE;
    }

    auto *client_app_state = new (mem) ClientAppState{};

    *appstate = client_app_state;

    // SDL_Surface *surface = NULL;
    // char *png_path = NULL;

    SDL_SetAppMetadata("Example Renderer Textures", "1.0", "com.example.renderer-textures");

    // TODO SDL logs?

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

    // /* Textures are pixel data that we upload to the video hardware for fast drawing. Lots of 2D
    //    engines refer to these as "sprites." We'll do a static texture (upload once, draw many
    //    times) with data from a png file. */

    // /* SDL_Surface is pixel data the CPU can access. SDL_Texture is pixel data the GPU can
    // access.
    //    Load a .png into a surface, move it to a texture from there. */
    // SDL_asprintf(&png_path, "%ssample.png",
    //              SDL_GetBasePath()); /* allocate a string of the full file path */
    // surface = SDL_LoadPNG(png_path);
    // if (!surface)
    // {
    //     SDL_Log("Couldn't load png: %s", SDL_GetError());
    //     return SDL_APP_FAILURE;
    // }

    // SDL_free(png_path); /* done with this, the file is loaded. */

    // texture_width = surface->w;
    // texture_height = surface->h;

    // texture = SDL_CreateTextureFromSurface(renderer, surface);
    // if (!texture)
    // {
    //     SDL_Log("Couldn't create static texture: %s", SDL_GetError());
    //     return SDL_APP_FAILURE;
    // }

    // SDL_DestroySurface(surface); /* done with this, the texture has a copy of the pixels now. */

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    auto *client_app_state = static_cast<ClientAppState *>(appstate);
    auto &input_event_adapter = client_app_state->input_event_adapter;
    auto &client_app = client_app_state->client_app;

    // TODO method name
    input_event_adapter.adapt<ClientApp>(*event, client_app);

    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    auto *client_app_state = static_cast<ClientAppState *>(appstate);
    auto &client_app = client_app_state->client_app;
    auto view = client_app.view();

    // TODO logic tick should happen in separate thread - 2 view buffers should be maintained, and
    // the view buffer should be swapped after the logic tick is complete. The rendering thread
    // should always render the current view buffer, and the logic thread should always update the
    // next view buffer. Each ECS comp should have a "tick" and an "updateView" / "view" method
    // Either that, or there is an ECS comp that runs last that updates the view buffer

    //     SDL_FRect dst_rect;
    //     const Uint64 now = SDL_GetTicks();

    //     /* we'll have some textures move around over a few seconds. */
    //     const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    //     const float scale = ((float)(((int)(now % 1000)) - 500) / 500.0f) * direction;

    //     /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); /* black, full alpha */
    SDL_RenderClear(renderer);                                   /* start with a blank
//     canvas. */

    //     /* Just draw the static texture a few times. You can think of it like a
    //        stamp, there isn't a limit to the number of times you can draw with it. */

    //     /* top left */
    //     dst_rect.x = (100.0f * scale);
    //     dst_rect.y = 0.0f;
    //     dst_rect.w = (float)texture_width;
    //     dst_rect.h = (float)texture_height;
    //     SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    //     /* center this one. */
    //     dst_rect.x = ((float)(WINDOW_WIDTH - texture_width)) / 2.0f;
    //     dst_rect.y = ((float)(WINDOW_HEIGHT - texture_height)) / 2.0f;
    //     dst_rect.w = (float)texture_width;
    //     dst_rect.h = (float)texture_height;
    //     SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    //     /* bottom right. */
    //     dst_rect.x = ((float)(WINDOW_WIDTH - texture_width)) - (100.0f * scale);
    //     dst_rect.y = (float)(WINDOW_HEIGHT - texture_height);
    //     dst_rect.w = (float)texture_width;
    //     dst_rect.h = (float)texture_height;
    //     SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // SDL_DestroyTexture(texture);

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
