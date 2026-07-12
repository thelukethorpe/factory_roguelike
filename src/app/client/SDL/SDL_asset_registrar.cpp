#include "SDL_asset_registrar.h"
#include "app/client/asset_metadata.h"

SDLAssetRegistrar::SDLAssetRegistrar(SDL_Renderer *renderer)
    : renderer_(renderer), player_textures_{}
{
    for (std::size_t i = 0; i < LoadoutId::NumLoadouts; ++i)
    {
        const auto loadout_id = static_cast<LoadoutId>(i);
        const auto texture_file_path = AssetMetadata::LOADOUT_TEXTURE_FILE_PATHS.at(loadout_id);
        player_textures_.at(i) = loadTextureFromFile(texture_file_path);
    }
}

SDLAssetRegistrar::~SDLAssetRegistrar()
{
    for (auto &texture : player_textures_)
    {
        if (texture.sdl_texture != nullptr)
        {
            SDL_DestroyTexture(texture.sdl_texture);
        }
    }
}

SDLAssetRegistrar::Texture SDLAssetRegistrar::getPlayerTexture(LoadoutId loadout_id) const
{
    return player_textures_.at(loadout_id);
}

SDLAssetRegistrar::Texture
SDLAssetRegistrar::loadTextureFromFile(const std::string_view &file_path) const
{
    char *full_file_path = nullptr;
    const char *base_path = SDL_GetBasePath();
    if (base_path == nullptr)
    {
        SDL_Log("Couldn't get base path: %s", SDL_GetError());
        return {}; // TODO
        // return nullptr;
    }

    if (SDL_asprintf(&full_file_path, "%sassets/%.*s.bmp", base_path,
                     static_cast<int>(file_path.size()), file_path.data()) < 0)
    {
        SDL_Log("Couldn't build file path");
        return {}; // TODO
        // return nullptr;
    }

    // TODO convert to SDL_LoadPNG using SDL_image
    auto *surface = SDL_LoadBMP(full_file_path);
    SDL_free(full_file_path);
    if (surface == nullptr)
    {
        SDL_Log("Couldn't load image: %s", SDL_GetError());
        return {}; // TODO
        // return nullptr;
    }

    const auto texture_width = surface->w;
    const auto texture_height = surface->h;

    auto *texture = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_DestroySurface(surface);
    if (texture == nullptr)
    {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return {}; // TODO
        // return nullptr;
    }

    return {.sdl_texture = texture, .width = texture_width, .height = texture_height};
}
