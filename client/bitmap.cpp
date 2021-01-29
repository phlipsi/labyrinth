#include <bitmap.hpp>

#include <renderer.hpp>
#include <surface.hpp>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>

#include <stdexcept>

namespace graphic {

surface load_image(const std::filesystem::path &source) {
    SDL_Surface *handle = IMG_Load(source.c_str());
    if (handle != nullptr) {
        return surface(handle);
    } else {
        throw std::runtime_error("Can't load image");
    }
}

}
