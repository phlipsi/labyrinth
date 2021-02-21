#include "bitmap.hpp"

#include "surface.hpp"

#include <SDL_image.h>
#include <SDL_surface.h>

#include <stdexcept>

namespace labyrinth { namespace client {

surface load_image(const std::filesystem::path &source) {
    SDL_Surface *handle = IMG_Load(source.u8string().c_str());
    if (handle != nullptr) {
        return surface(handle);
    } else {
        throw std::runtime_error("Can't load image: " + source.string());
    }
}

} }
