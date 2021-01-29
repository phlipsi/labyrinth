#pragma once

#include <cstdint>
#include <vector>

#include "event.hpp"

namespace labyrinth { namespace client {

class game;
class renderer;

class game_object {
public:
    virtual ~game_object() = default;

    virtual void draw(game &g, const renderer &r) const = 0;
    virtual void update(game &g, uint32_t elapsed_ticks) = 0;
    virtual bool check_collisions(game &g, const game_object &other) const = 0;
    virtual event::handling_result handle_event(game &g, const event &e) = 0;

    virtual std::vector<game_object *> collect_leafs();
};

} }
