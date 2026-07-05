#ifndef SDL_ASSET_REGISTRAR_H
#define SDL_ASSET_REGISTRAR_H

#include <SDL3/SDL.h>
#include <core/game/loadout.h>
#include <string>

class SDLAssetRegistrar
{
  public:
    SDLAssetRegistrar(SDL_Renderer *renderer) : renderer_(renderer) {}

    ~SDLAssetRegistrar() = default;
    SDLAssetRegistrar(const SDLAssetRegistrar &) = delete;
    SDLAssetRegistrar(SDLAssetRegistrar &&) = delete;
    SDLAssetRegistrar &operator=(const SDLAssetRegistrar &) = delete;
    SDLAssetRegistrar &operator=(SDLAssetRegistrar &&) = delete;

    [[nodiscard]] SDL_Texture *getPlayerTexture(LoadoutId loadout_id) const;

  private:
    SDL_Renderer *renderer_;

    // TODO this is all readonly - should we add const to more struct fields across the codebase?
    struct Texture
    {
        SDL_Texture *sdl_texture;
        int width;
        int height;
    };

    [[nodiscard]] Texture loadTextureFromFile(const std::string &file_path) const;
};

#endif
