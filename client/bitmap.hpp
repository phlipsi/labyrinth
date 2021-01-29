#pragma once

#include <drawable.hpp>
#include <texture.hpp>

#include <filesystem>

namespace graphic {

class surface;

surface load_image(const std::filesystem::path &source);

}
