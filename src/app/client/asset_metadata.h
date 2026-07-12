#ifndef ASSET_METADATA_H
#define ASSET_METADATA_H

#include <core/game/loadout.h>
#include <stdexcept>
#include <string>

namespace AssetMetadata
{
std::string getLoadoutTextureFilePath(LoadoutId loadout_id)
{
    switch (loadout_id)
    {
    case LoadoutId::Warper:
        return "player/warper";
    case LoadoutId::NumLoadouts:
        throw std::invalid_argument("Invalid loadout id: NumLoadouts");
    }
}
} // namespace AssetMetadata

#endif
