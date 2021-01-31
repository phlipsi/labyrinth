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
    bat(r.create_texture_from_surface(load_image(ASSETS_DIRECTORY "/bat.png"))),
    light(r.create_texture_from_surface(load_image(ASSETS_DIRECTORY "/light.png"))),
    time(0)
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

    for (int u = 0; u < 7; ++u) {
        for (int v = 0; v < 5; ++v) {
            r.copy(walls, rectangle{ 0x10 * 128, 0, 128, 128 }, rectangle{ u * 256, v * 256, 256, 256 }, 0.0, point<int>{ 0, 0 }, false);
        }
    }
    for (int u = 0; u < width; ++u) {
        for (int v = 0; v < height; ++v) {
            const int pos = u * du + v * dv + w * dw;
            const int unsigned tile = tile_index(s->tiles[pos], perspective);
            r.copy(walls, rectangle{ static_cast<int>(tile) * 128, 0, 128, 128 }, rectangle{ u * 256, v * 256, 256, 256 }, 0.0, point<int>{ 0, 0 }, false);
        }
    }
    const int goal_u = perspective == 0 ? s->goal_x : s->goal_z;
    const int goal_v = s->goal_y;
    const int goal_w = perspective == 0 ? s->goal_z : s->goal_x;
    const int bat_index = time / 300;
    if (perspective == 0) {
        r.copy(bat, rectangle{ bat_index * 128, 0, 128, 128 }, rectangle{ you_u * 256 + 64, you_v * 256 + 64, 128, 128 }, 0.0, point<int>{ 0, 0 }, false);
    } else {
        r.copy(bat, rectangle{ bat_index * 128, 0, 128, 128 }, rectangle{ you_u * 256 + 64, you_v * 256 + 64, 128, 128 }, 0.0, point<int>{ 0, 0 }, true);
    }
    if (goal_w == w) {
        r.copy(bat, rectangle{ 512, 0, 128, 128 }, rectangle{ goal_u * 256, goal_v * 256, 256, 256 }, 0.0, point<int>{ 0, 0 }, false);
    }
    r.copy(light, rectangle{ 0, 0, 80, 60 }, rectangle { 128 + you_u * 256 - 800, 128 + you_v * 256 - 600, 1600, 1200 }, 0.0, point<int>{ 0, 0 }, false);
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

void level::update(game &g, uint32_t elapsed_ticks) {
    time += elapsed_ticks;
    if (time > 1200) {
        time = 0;
    }
}

bool level::check_collisions(game &g, const game_object &other) const {
    return false;
}

event::handling_result level::handle_event(game &g, const event &e) {
    return event::handling_result::CONTINUE;
}

}
