#pragma once

#include <SDL_render.h>

namespace labyrinth { namespace client {

class texture {
public:
    texture();
    explicit texture(SDL_Texture *handle);
    explicit texture(SDL_Texture *handle, unsigned int width, unsigned int height);
    ~texture();

    texture(const texture &) = delete;
    texture &operator = (const texture &) = delete;

    texture(texture &&other);
    texture &operator = (texture &&other);

    void swap(texture &other);

    SDL_Texture *get_handle() const;

    unsigned int get_width() const { return width; }
    unsigned int get_height() const { return height; }

private:
    SDL_Texture *handle;
    unsigned int width;
    unsigned int height;
};

} }
