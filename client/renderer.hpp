#pragma once

#include <SDL2/SDL.h>

#include "point.hpp"

namespace labyrinth { namespace client {

class rectangle;
class surface;
class texture;

class renderer {
public:
    enum class blend_mode {
        NONE,
        BLEND,
        ADD,
        MOD
    };

    renderer() = default;
    virtual ~renderer() = default;

    renderer(const renderer &) = delete;
    renderer &operator = (const renderer &) = delete;

    virtual texture create_texture_from_surface(const surface &source) const = 0;

    virtual int set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const = 0;
    virtual int set_blend_mode(blend_mode mode) const = 0;
    virtual int clear() const = 0;

    virtual int copy(const texture &source, const rectangle &source_rect,
                     const rectangle &dest_rect,
                     float angle, const point<int> &mid,
                     bool flip) const = 0;

    virtual int line(const point<int> &p, const point<int> &q) const = 0;

    virtual int fill_rect(const rectangle &r) const = 0;

    virtual void present() const = 0;

    void draw_rect(const rectangle &r, float angle, const point<int> &mid) const;
};

} }
