#include "SDL_asset_registrar.h"

SDLAssetRegistrar::Texture
SDLAssetRegistrar::loadTextureFromFile(const std::string &file_path) const
{
    char *full_file_path = nullptr;
    SDL_asprintf(&full_file_path, "%s%s.bmp", SDL_GetBasePath(), file_path.c_str());

    // TODO convert to SDL_LoadPNG using SDL_image
    auto *surface = SDL_LoadBMP(full_file_path);
    if (surface == nullptr)
    {
        SDL_Log("Couldn't load image: %s", SDL_GetError());
        return nullptr;
    }

    SDL_free(full_file_path); /* done with this, the file is loaded. */

    const auto texture_width = surface->w;
    const auto texture_height = surface->h;

    auto *texture = SDL_CreateTextureFromSurface(renderer_, surface);
    if (texture == nullptr)
    {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return nullptr;
    }

    SDL_DestroySurface(surface);
    return {.sdl_texture = texture, .width = texture_width, .height = texture_height};
}
