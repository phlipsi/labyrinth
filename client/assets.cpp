#include "assets.hpp"

#include <config.hpp>

namespace labyrinth::client {

namespace {

std::filesystem::path get_assets_root_path(const std::filesystem::path &self) {
    std::filesystem::path result(ASSETS_DIRECTORY);
    if (result.is_absolute()) {
        return result;
    } else {
        return absolute(self.parent_path() / result);
    }
}

}

assets::assets(const std::filesystem::path &self)
  : assets_root(get_assets_root_path(self))
{ }

std::filesystem::path assets::get_asset_path(const std::filesystem::path &asset) const {
    return assets_root / asset;
}

}
