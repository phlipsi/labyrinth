#pragma once

#include <vector>

namespace labyrinth::common {

// i.e. your head movement
enum class movement : char {
    NONE,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FORTH,
    BACK
};

std::vector<char> write_movement(movement m);
movement read_movement(const std::vector<char> &payload);

}
