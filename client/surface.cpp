#include "surface.hpp"

namespace labyrinth { namespace client {

surface::surface(SDL_Surface *handle) : handle(handle) { }

surface::~surface() {
    if (handle != nullptr) {
        SDL_FreeSurface(handle);
        handle = nullptr;
    }
}

surface::surface(surface &&other) : handle(other.handle) {
    other.handle = nullptr;
}

surface &surface::operator = (surface &&other) {
    if (this != &other) {
        swap(other);
    }
    return *this;
}

void surface::swap(surface &other) {
    using std::swap;
    swap(handle, other.handle);
}

SDL_Surface *surface::get_handle() const {
    return handle;
}

int surface::get_width() const {
    return handle->w;
}

int surface::get_height() const {
    return handle->h;
}

} }
