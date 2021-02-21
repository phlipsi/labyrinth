#include "font.hpp"

#include "renderer.hpp"
#include "surface.hpp"
#include "texture.hpp"

#include <stdexcept>

namespace labyrinth::client {

font::font(std::filesystem::path f, unsigned int size)
  : f(TTF_OpenFont(f.u8string().c_str(), size))
{
    if (this->f == nullptr) {
        throw std::runtime_error("Can't open font " + f.string() + ": " + TTF_GetError());
    }
}

font::~font() {
    TTF_CloseFont(f);
}

texture font::render(const renderer &r,
                     const std::string &text,
                     uint8_t r1,
                     uint8_t g1,
                     uint8_t b1,
                     uint8_t a1) const
{
    SDL_Color c{ r1, g1, b1, a1 };
    // SDL_Color b{ 0, 0, 0, 0x01 };
    surface s(TTF_RenderText_Solid(f, text.c_str(), c)); // , b));
    if (s.get_handle() == nullptr) {
        SDL_Log("Error rendering text: %s", TTF_GetError());
        throw std::runtime_error("Error rendering text");
    }
    return r.create_texture_from_surface(s);
}

}
