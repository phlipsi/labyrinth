#pragma once

#include <SDL.h>

#include <cstdint>
#include <memory>
#include <string>

namespace labyrinth { namespace client {

class renderer;

class window {
public:
    window(const std::string &title, int x, int y, int w, int h, uint32_t flags);
    ~window();

    const renderer &get_renderer() const;
    SDL_Window *get_handle() const;
private:
    SDL_Window *handle;
    std::unique_ptr<renderer> r;
};

} }
