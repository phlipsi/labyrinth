#include <game_object.hpp>

namespace graphic {

std::vector<game_object *> game_object::collect_leafs() {
    return { this };
}

}
