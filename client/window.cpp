#include "window.hpp"

#include <stdexcept>

#include "renderer.hpp"
#include "point.hpp"
#include "rectangle.hpp"
#include "surface.hpp"
#include "texture.hpp"

namespace labyrinth { namespace client {

namespace {

class sdl_renderer : public renderer {
public:
    sdl_renderer(SDL_Renderer *handle);
    virtual ~sdl_renderer();

    virtual texture create_texture_from_surface(const surface &s) const;

    virtual int set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const;
    virtual int set_blend_mode(blend_mode mode) const;
    virtual int clear() const;

    virtual int copy(const texture &source, const rectangle &source_rect,
                     const rectangle &dest_rect,
                     float angle, const point<int> &mid) const;

    virtual int line(const point<int> &p, const point<int> &q) const;

    virtual int fill_rect(const rectangle &r) const;

    virtual void present() const;
private:
    SDL_Renderer *handle;
};

sdl_renderer::sdl_renderer(SDL_Renderer *handle) : handle(handle) {
    if (handle == nullptr) {
        throw std::runtime_error("Invalid renderer handle");
    }
}

sdl_renderer::~sdl_renderer() {
    if (handle != nullptr) {
        SDL_DestroyRenderer(handle);
    }
}

texture sdl_renderer::create_texture_from_surface(const surface &s) const {
    SDL_Texture *const t = SDL_CreateTextureFromSurface(handle, s.get_handle());
    if (t != nullptr) {
        return texture(t);
    } else {
        throw std::runtime_error("Can't create texture from surface");
    }
}

int sdl_renderer::set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const {
    return SDL_SetRenderDrawColor(handle, r, g, b, a);
}

int sdl_renderer::set_blend_mode(blend_mode mode) const {
    switch (mode) {
    case blend_mode::NONE:
        return SDL_SetRenderDrawBlendMode(handle, SDL_BLENDMODE_NONE);
    case blend_mode::BLEND:
        return SDL_SetRenderDrawBlendMode(handle, SDL_BLENDMODE_BLEND);
    case blend_mode::ADD:
        return SDL_SetRenderDrawBlendMode(handle, SDL_BLENDMODE_ADD);
    case blend_mode::MOD:
        return SDL_SetRenderDrawBlendMode(handle, SDL_BLENDMODE_MOD);
    default:
        throw std::runtime_error("Invalid blend mode");
    }
}

int sdl_renderer::clear() const {
    return SDL_RenderClear(handle);
}

int sdl_renderer::copy(const texture &source, const rectangle &source_rect,
                       const rectangle &dest_rect,
                       float angle, const point<int> &mid) const
{
    const SDL_Rect source_rect2 = source_rect.get_sdl_rect();
    const SDL_Rect dest_rect2 = dest_rect.get_sdl_rect();
    const SDL_Point mid2 { mid.x, mid.y };
    return SDL_RenderCopyEx(handle, source.get_handle(), &source_rect2, &dest_rect2, angle, &mid2, SDL_FLIP_NONE);
}

int sdl_renderer::line(const point<int> &p, const point<int> &q) const {
    return SDL_RenderDrawLine(handle, p.x, p.y, q.x, q.y);
}

int sdl_renderer::fill_rect(const rectangle &r) const {
    SDL_Rect r2 = r.get_sdl_rect();
    return SDL_RenderFillRect(handle, &r2);
}

void sdl_renderer::present() const {
    SDL_RenderPresent(handle);
}

}

window::window(const std::string &title, int x, int y, int w, int h, uint32_t flags)
  : handle(SDL_CreateWindow(title.c_str(), x, y, w, h, flags)),
    r(new sdl_renderer(SDL_CreateRenderer(handle, -1, SDL_RENDERER_ACCELERATED)))
{
    if (handle == nullptr) {
        throw std::runtime_error("Error creating window");
    }
}

window::~window() {
    SDL_DestroyWindow(handle);
}

const renderer &window::get_renderer() const {
    return *r;
}

SDL_Window *window::get_handle() const {
    return handle;
}

} }
