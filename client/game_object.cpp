#include "game_object.hpp"

namespace labyrinth::client {

std::vector<game_object *> game_object::collect_leafs() {
    return { this };
}

}
