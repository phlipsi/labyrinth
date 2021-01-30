#include "level.hpp"

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

level::level(const renderer &r, int perspective, common::state &s)
  : s(&s),
    perspective(perspective),
    walls(r.create_texture_from_surface(load_image(ASSETS_DIRECTORY "/walls.png"))),
    items(r.create_texture_from_surface(load_image(ASSETS_DIRECTORY "/items.png"))),
    light(r.create_texture_from_surface(load_image(ASSETS_DIRECTORY "/light.png")))
{ }

void level::draw(game &g, const renderer &r) const {
    const int width = perspective == 0 ? s->width : s->depth;
    const int height = s->height;
    const int du = perspective == 0 ? 1 : s->width * s->height;
    const int dv = s->width;
    const int dw = perspective == 0 ? s->width * s->height : 1;
    const int w = perspective == 0 ? s->z : s->x;
    const int you_u = perspective == 0 ? s->x : s->z;
    const int you_v = s->y;

    for (int u = 0; u < std::min(13, you_u + 4); ++u) {
        for (int v = 0; v < std::min(10, you_v + 3); ++v) {
            r.copy(walls, rectangle{ 0x10 * 64, 0, 64, 64 }, rectangle{ u * 128, v * 128, 128, 128 }, 0.0, point<int>{ 0, 0 });
        }
    }
    for (int u = 0; u < width; ++u) {
        for (int v = 0; v < height; ++v) {
            const int pos = u * du + v * dv + w * dw;
            const int unsigned tile = tile_index(s->tiles[pos], perspective);
            r.copy(walls, rectangle{ static_cast<int>(tile) * 64, 0, 64, 64 }, rectangle{ (u + 1) * 128, (v + 1) * 128, 128, 128 }, 0.0, point<int>{ 0, 0 });
        }
    }
    const int goal_u = perspective == 0 ? s->goal_x : s->goal_z;
    const int goal_v = s->goal_y;
    const int goal_w = perspective == 0 ? s->goal_z : s->goal_x;
    if (perspective == 0) {
        r.copy(items, rectangle{ 64, 0, 64, 64 }, rectangle{ (you_u + 1) * 128, (you_v + 1) * 128, 128, 128 }, 0.0, point<int>{ 0, 0 });
    } else {
        r.copy(items, rectangle{ 0, 0, 64, 64 }, rectangle{ (you_u + 1) * 128, (you_v + 1) * 128, 128, 128 }, 0.0, point<int>{ 0, 0 });
    }
    if (goal_w == w) {
        r.copy(items, rectangle{ 128, 0, 64, 64 }, rectangle{ (goal_u + 1) * 128, (goal_v + 1) * 128, 128, 128 }, 0.0, point<int>{ 0, 0 });
    }
    r.copy(light, rectangle{ 0, 0, 800, 600 }, rectangle { 64 + (you_u + 1) * 128 - 400, 64 + (you_v + 1) * 128 - 300, 800, 600 }, 0.0, point<int>{ 0, 0 });
}


bool level::move_left() {
    unsigned int tile = s->tiles[s->x + s->width * s->y + s->width * s->height * s->z];
    if (perspective == 0) {
        if (!common::tile_has_left_wall(tile)) {
            --s->x;
            return true;
        }
    } else {
        if (!common::tile_has_front_wall(tile)) {
            --s->z;
            return true;
        }
    }
    return false;
}

bool level::move_right() {
    unsigned int tile = s->tiles[s->x + s->width * s->y + s->width * s->height * s->z];
    if (perspective == 0) {
        if (!common::tile_has_right_wall(tile)) {
            ++s->x;
            return true;
        }
    } else {
        if (!common::tile_has_back_wall(tile)) {
            ++s->z;
            return true;
        }
    }
    return false;
}

bool level::move_up() {
    unsigned int tile = s->tiles[s->x + s->width * s->y + s->width * s->height * s->z];
    if (!common::tile_has_top_wall(tile)) {
        --s->y;
        return true;
    }
    return false;
}

bool level::move_down() {
    unsigned int tile = s->tiles[s->x + s->width * s->y + s->width * s->height * s->z];
    if (!common::tile_has_bottom_wall(tile)) {
        ++s->y;
        return true;
    }
    return false;
}

void level::update(game &g, uint32_t elapsed_ticks) { }

bool level::check_collisions(game &g, const game_object &other) const {
    return false;
}

event::handling_result level::handle_event(game &g, const event &e) {
    return event::handling_result::CONTINUE;
}

}
