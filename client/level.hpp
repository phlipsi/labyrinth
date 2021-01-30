#pragma once

#include "game_object.hpp"
#include "texture.hpp"

#include <state.hpp>

#include <array>

namespace labyrinth::client {

class level : game_object {
public:
    explicit level(const renderer &r, int perspective, common::state &s);

    virtual void draw(game &g, const renderer &r) const override;
    virtual void update(game &g, uint32_t elapsed_ticks) override;
    virtual bool check_collisions(game &g, const game_object &other) const override;
    virtual event::handling_result handle_event(game &g, const event &e) override;

    bool move_left();
    bool move_right();
    bool move_up();
    bool move_down();

    void set_perspective(int perspective) { this->perspective = perspective; }
    int get_perspective() const { return perspective; }

private:
    common::state *s;
    int perspective;
    texture walls;
    texture items;
    texture light;
};

}
