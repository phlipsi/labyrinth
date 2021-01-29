#include "texture.hpp"

namespace labyrinth { namespace client {

texture::texture(SDL_Texture *handle) : handle(handle) { }
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
