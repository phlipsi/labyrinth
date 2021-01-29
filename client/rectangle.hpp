#pragma once

#include <SDL2/SDL_rect.h>

namespace labyrinth { namespace client {

struct rectangle {
    int x;
    int y;
    int w;
    int h;

    SDL_Rect get_sdl_rect() const {
        return { x, y, w, h };
    }
};

} }
