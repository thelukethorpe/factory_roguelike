#ifndef SDL_ASSET_REGISTRAR_H
#define SDL_ASSET_REGISTRAR_H

#include <SDL3/SDL.h>
#include <array>
#include <core/game/loadout.h>
#include <string>

class SDLAssetRegistrar
{
  public:
    // TODO this is all readonly - should we add const to more struct fields across the codebase?
    struct Texture
    {
        SDL_Texture *sdl_texture;
        int width;
        int height;
    };

    SDLAssetRegistrar(SDL_Renderer *renderer);
    ~SDLAssetRegistrar() = default; // TODO free textures in destructor SDL_DestroyTexture
    SDLAssetRegistrar(const SDLAssetRegistrar &) = delete;
    SDLAssetRegistrar(SDLAssetRegistrar &&) = delete;
    SDLAssetRegistrar &operator=(const SDLAssetRegistrar &) = delete;
    SDLAssetRegistrar &operator=(SDLAssetRegistrar &&) = delete;

    [[nodiscard]] Texture getPlayerTexture(LoadoutId loadout_id) const;

  private:
    SDL_Renderer *renderer_;

    std::array<Texture, LoadoutId::NumLoadouts> player_textures_;

    [[nodiscard]] Texture loadTextureFromFile(const std::string &file_path) const;
};

#endif
