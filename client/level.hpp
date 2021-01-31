#pragma once

#include "font.hpp"
#include "game_object.hpp"
#include "texture.hpp"

#include <state.hpp>

#include <cstdint>
#include <array>

namespace labyrinth::client {

class level : game_object {
public:
    explicit level(const renderer &r);

    virtual void draw(game &g, const renderer &r) const override;
    virtual void update(game &g, uint32_t elapsed_ticks) override;
    virtual bool check_collisions(game &g, const game_object &other) const override;
    virtual event::handling_result handle_event(game &g, const event &e) override;

    void set_perspective(int perspective) { this->perspective = perspective; }
    int get_perspective() const { return perspective; }

    void move_to_state(common::state new_state);

private:
    font small;
    font large;
    common::state s;
    int perspective;
    texture walls;
    texture bat;
    texture light;
    texture waiting;
    texture lvl;
    uint32_t time;
};

}
