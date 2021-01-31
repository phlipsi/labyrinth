#pragma once

#include "point.hpp"

#include <SDL2/SDL_ttf.h>

#include <cstdint>
#include <filesystem>
#include <string>

namespace labyrinth::client {

class renderer;
class texture;

class font {
public:
    font(std::filesystem::path f, unsigned int size);
    ~font();

    texture render(const renderer &r,
                   const std::string &text,
                   uint8_t r1,
                   uint8_t g1,
                   uint8_t b1,
                   uint8_t a1) const;

private:
    TTF_Font *f;
};

}
