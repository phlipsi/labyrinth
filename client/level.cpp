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

level::level(const renderer &r)
  : small(ASSETS_DIRECTORY "/Avara.ttf", 32),
    large(ASSETS_DIRECTORY "/Avara.ttf", 64),
    player_x(current.x), player_y(current.y), player_z(current.z),
    perspective(0),
    walls(r.create_texture_from_surface(load_image(ASSETS_DIRECTORY "/walls.png"))),
    bat(r.create_texture_from_surface(load_image(ASSETS_DIRECTORY "/bat.png"))),
    light(r.create_texture_from_surface(load_image(ASSETS_DIRECTORY "/light.png"))),
    waiting(small.render(r, "Waiting for other player...", 0x96, 0x66, 0x66, 0x66)),
    found_texture_decay(0.0),
    found_texture(large.render(r, "Found!", 0x96, 0x96, 0x96, 0xff)),
    title_texture(small.render(r, current.title, 0x96, 0x96, 0x96, 0x66)),
    time(0)
{ }

void level::draw(game &g, const renderer &r) const {
    const int width = perspective == 0 ? current.width : current.depth;
    const int height = current.height;
    const int du = perspective == 0 ? 1 : current.width * current.height;
    const int dv = current.width;
    const int dw = perspective == 0 ? current.width * current.height : 1;
    const float w = perspective == 0 ? current.z : current.x;
    const float you_u = perspective == 0 ? player_x : player_z;
    const float you_v = player_y;
    const float you_w = perspective == 0 ? player_z : player_x;

    for (int u = 0; u < 7; ++u) {
        for (int v = 0; v < 5; ++v) {
            r.copy(walls, rectangle{ 0x10 * 128, 0, 128, 128 }, rectangle{ u * 256, v * 256, 256, 256 }, 0.0, point<int>{ 0, 0 }, false);
        }
    }
    for (int u = 0; u < width; ++u) {
        for (int v = 0; v < height; ++v) {
            const int pos = u * du + v * dv + w * dw;
            const int unsigned tile = tile_index(current.tiles[pos], perspective);
            r.copy(walls, rectangle{ static_cast<int>(tile) * 128, 0, 128, 128 }, rectangle{ u * 256, v * 256, 256, 256 }, 0.0, point<int>{ 0, 0 }, false);
        }
    }
    const int goal_u = perspective == 0 ? current.goal_x : current.goal_z;
    const int goal_v = current.goal_y;
    const int goal_w = perspective == 0 ? current.goal_z : current.goal_x;
    if (goal_w == w) {
        r.copy(bat, rectangle{ 512, 0, 128, 128 }, rectangle{ goal_u * 256, goal_v * 256, 256, 256 }, 0.0, point<int>{ 0, 0 }, false);
    }
    const int bat_index = time / 300;
    const bool flip = perspective == 1;
    const float scale = 100 * (perspective == 0 ? player_z - current.z : player_x - current.x);
    r.copy(bat, rectangle{ bat_index * 128, 0, 128, 128 },
        rectangle{ static_cast<int>(you_u * 256 + 64 - scale / 2), static_cast<int>(you_v * 256 + 64 - scale / 2), static_cast<int>(128 + scale), static_cast<int>(128 + scale)},
        0.0, point<int>{ 0, 0 }, flip);
    r.copy(light, rectangle{ 0, 0, 80, 60 },
           rectangle{ static_cast<int>(128 + you_u * 256 - 800), static_cast<int>(128 + you_v * 256 - 600), 1600, 1200 },
           0.0, point<int>{ 0, 0 }, false);
    /*r.copy(waiting, rectangle{ 0, 0, static_cast<int>(waiting.get_width()), static_cast<int>(waiting.get_height()) },
           rectangle{ static_cast<int>(800 - waiting.get_width()) / 2, static_cast<int>(600 - waiting.get_height()) / 2, static_cast<int>(waiting.get_width()), static_cast<int>(waiting.get_height()) },
           0.0,
           point<int>{ 0, 0 },
           false);*/
    r.copy(title_texture,
           rectangle{ 0, 0, static_cast<int>(title_texture.get_width()), static_cast<int>(title_texture.get_height()) },
           rectangle{ 10, static_cast<int>(600 - title_texture.get_height()) - 10, static_cast<int>(title_texture.get_width()), static_cast<int>(title_texture.get_height()) },
           0.0,
           point<int>{ 0, 0 },
           false);
    SDL_SetTextureAlphaMod(found_texture.get_handle(), static_cast<uint8_t>(255.0 * found_texture_decay));
    r.copy(found_texture,
           rectangle{ 0, 0, static_cast<int>(found_texture.get_width()), static_cast<int>(found_texture.get_height()) },
           rectangle{ static_cast<int>(800 - 20 * (1.0 - found_texture_decay) * found_texture.get_width()) / 2, static_cast<int>(600 - 20 * (1.0 - found_texture_decay) * found_texture.get_height()) / 2, static_cast<int>(20 * (1.0 - found_texture_decay) * found_texture.get_width()), static_cast<int>(20 * (1.0 - found_texture_decay) * found_texture.get_height()) },
           0.0,
           point<int>{ 0, 0 },
           false);
}

void level::update(game &g, const renderer &r, uint32_t elapsed_ticks) {
    if (found_texture_decay > 0) {
        found_texture_decay -= elapsed_ticks * 0.001;
    }
    if (found_texture_decay < 0) {
        found_texture_decay = 0;
    }
    time += elapsed_ticks;
    if (time > 1200) {
        current = next;
        time = 0;
    }
    int diff_x = 0;
    int diff_y = 0;
    int diff_z = 0;
    std::tie(diff_x, diff_y, diff_z) = current.get_movement_diff(next);
    if (diff_x != 0) {
        player_x = current.x * (1200 - time) / 1200.0 + next.x * time / 1200.0;
    } else {
        player_x = current.x;
    }
    if (diff_y != 0) {
        player_y = current.y * (1200 - time) / 1200.0 + next.y * time / 1200.0;
    } else {
        player_y = current.y;
    }
    if (diff_z != 0) {
        player_z = current.z * (1200 - time) / 1200.0 + next.z * time / 1200.0;
    } else {
        player_z = current.z;
    }
    if (current.title != next.title) {
        title = next.title;
        title_texture = small.render(r, title, 0x96, 0x96, 0x96, 0x66);
    }
    //SDL_SetTextureAlphaMod(found_texture.get_handle(), static_cast<int8_t>(256.0 * (sin(M_PI * time / 1200.0) + 1.0)));
    SDL_SetTextureAlphaMod(waiting.get_handle(), static_cast<int8_t>(256.0 * (sin(M_PI * time / 1200.0) + 1.0)));
}

void level::move_to_state(common::state new_state) {
    this->next = new_state;
    if (!current.win() && next.win() && found_texture_decay < 0.001) {
        found_texture_decay = 1.0;
    }
}

bool level::check_collisions(game &g, const game_object &other) const {
    return false;
}

event::handling_result level::handle_event(game &g, const event &e) {
    return event::handling_result::CONTINUE;
}

}
