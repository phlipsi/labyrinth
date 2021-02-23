#pragma once

#include <filesystem>

namespace labyrinth::client {

class assets {
public:
    explicit assets(const std::filesystem::path &self);

    std::filesystem::path get_asset_path(const std::filesystem::path &asset) const;
private:
    std::filesystem::path assets_root;
};

}
