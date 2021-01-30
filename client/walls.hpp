#pragma once

#include "game_object.hpp"
#include "texture.hpp"

#include <state.hpp>

#include <array>

namespace labyrinth::client {

class walls : game_object {
public:
    explicit walls(const renderer &r, int perspective, common::state &s);

    virtual void draw(game &g, const renderer &r) const override;
    virtual void update(game &g, uint32_t elapsed_ticks) override;
    virtual bool check_collisions(game &g, const game_object &other) const override;
    virtual event::handling_result handle_event(game &g, const event &e) override;

private:
    common::state *s;
    int perspective;
    texture t; 
};

}
