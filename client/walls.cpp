#include "walls.hpp"

#include "texture.hpp"
#include "bitmap.hpp"
#include "point.hpp"
#include "rectangle.hpp"
#include "renderer.hpp"
#include "surface.hpp"

#include <config.hpp>

namespace labyrinth::client {

namespace {

unsigned int tile_index(unsigned int tile, int perspective) {
    if (perspective == 0) {
        return tile & 0x0f;
    } else {
        return ((tile & 0x30) >> 4) | (tile & 0x0c);
    }
}

}

walls::walls(const renderer &r, int perspective, common::state &s)
  : s(&s),
    perspective(perspective),
    t(r.create_texture_from_surface(load_image(ASSETS_DIRECTORY "/walls.png")))
{ }

void walls::draw(game &g, const renderer &r) const {
    const int width = perspective == 0 ? s->width : s->depth;
    const int height = s->height;
    const int du = perspective == 0 ? 1 : s->width * s->height;
    const int dv = s->width;
    const int dw = perspective == 0 ? s->width * s->height : 1;
    const int w = perspective == 0 ? s->z : s->x;
    for (int u = 0; u < width; ++u) {
        for (int v = 0; v < height; ++v) {
            const int pos = u * du + v * dv + w * dw;
            const int unsigned tile = tile_index(s->tiles[pos], perspective);
            r.copy(t, rectangle{ static_cast<int>(tile) * 64, 0, 64, 64 }, rectangle{ u * 128, v * 128, 128, 128 }, 0.0, point<int>{ 0, 0 });
        }
    }
}

void walls::update(game &g, uint32_t elapsed_ticks) { }

bool walls::check_collisions(game &g, const game_object &other) const {
    return false;
}

event::handling_result walls::handle_event(game &g, const event &e) {
    return event::handling_result::CONTINUE;
}

}
