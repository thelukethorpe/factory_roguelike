#ifndef ASSET_METADATA_H
#define ASSET_METADATA_H

#include <array>
#include <core/game/loadout.h>
#include <string_view>

namespace AssetMetadata
{
constexpr std::array<std::string_view, LoadoutId::NumLoadouts> LOADOUT_TEXTURE_FILE_PATHS{
#define F(name) "player/" #name,
    LOADOUT_VALUES
#undef F
};
} // namespace AssetMetadata

#endif
