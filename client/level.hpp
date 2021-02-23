#pragma once

#include "font.hpp"
#include "game_object.hpp"
#include "texture.hpp"

#include <state.hpp>

#include <cstdint>
#include <array>

namespace labyrinth::client {

class assets;

class level {
public:
    explicit level(const assets &a, const renderer &r);

    virtual void draw(game &g, const renderer &r) const;
    virtual void update(game &g, const renderer &r, uint32_t elapsed_ticks);
    virtual bool check_collisions(game &g, const game_object &other) const;
    virtual event::handling_result handle_event(game &g, const event &e);

    void set_perspective(int perspective) { this->perspective = perspective; }
    int get_perspective() const { return perspective; }

    void move_to_state(common::state new_state);

private:
    font small;
    font large;
    common::state current;
    common::state next;
    std::string title;
    float player_x;
    float player_y;
    float player_z;
    int perspective;
    texture walls;
    texture bat;
    texture light;
    texture waiting;
    float found_texture_decay;
    texture found_texture;
    texture title_texture;
    texture won_texture;
    uint32_t time;
};

}
