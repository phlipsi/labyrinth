#include <movement.hpp>

namespace labyrinth::common {

std::vector<char> write_movement(movement m) {
    return { static_cast<char>(m) };
}

movement read_movement(const std::vector<char> &payload) {
    if (!payload.empty()) {
        return movement(payload[0]);
    } else {
        return movement::NONE;
    }
}

}
