#pragma once

#include <SDL2/SDL_surface.h>

namespace labyrinth { namespace client {

class surface {
public:
    explicit surface(SDL_Surface *handle);
    ~surface();

    surface(const surface &) = delete;
    surface &operator = (const surface &) = delete;

    surface(surface &&other);
    surface &operator = (surface &&other);

    void swap(surface &other);

    SDL_Surface *get_handle() const;

    int get_height() const;
    int get_width() const;

private:
    SDL_Surface *handle;
};

} }
