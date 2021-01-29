#pragma once

#include <filesystem>

namespace labyrinth { namespace client {

class surface;

surface load_image(const std::filesystem::path &source);

} }
