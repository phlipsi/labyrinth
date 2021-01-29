#pragma once

#include <SDL2/SDL_render.h>

namespace labyrinth { namespace client {

class texture {
public:
    explicit texture(SDL_Texture *handle);
    ~texture();

    texture(const texture &) = delete;
    texture &operator = (const texture &) = delete;

    texture(texture &&other);
    texture &operator = (texture &&other);

    void swap(texture &other);

    SDL_Texture *get_handle() const;

private:
    SDL_Texture *handle;
};

} }
