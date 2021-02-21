#include "texture.hpp"

#include <algorithm>

namespace labyrinth { namespace client {

texture::texture() : handle(nullptr) { }

texture::texture(SDL_Texture *handle) : handle(handle), width(0), height(0) { }

texture::texture(SDL_Texture *handle,
                 unsigned int width,
                 unsigned int height)
  : handle(handle), width(width), height(height) { }

texture::~texture() {
    if (handle != nullptr) {
        SDL_DestroyTexture(handle);
        handle = nullptr;
    }
}

texture::texture(texture &&other) : handle(other.handle) {
    other.handle = nullptr;
}

texture &texture::operator = (texture &&other) {
    if (this != &other) {
        swap(other);
    }
    return *this;
}

void texture::swap(texture &other) {
    using std::swap;
    swap(handle, other.handle);
}

SDL_Texture *texture::get_handle() const {
    return handle;
}

} }
