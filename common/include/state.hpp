#pragma once

#include <vector>

namespace labyrinth { namespace common {

struct state {
    state();
    state(unsigned int width,
          unsigned int height,
          unsigned int depth,
          std::vector<unsigned int> tiles,
          unsigned int x,
          unsigned int y,
          unsigned int z,
          unsigned int goal_x,
          unsigned int goal_y,
          unsigned int goal_z);

    void read_from(const std::vector<char> &payload);
    std::vector<char> write_to() const;

    unsigned int width;
    unsigned int height;
    unsigned int depth;
    std::vector<unsigned int> tiles;
    unsigned int x;
    unsigned int y;
    unsigned int z;
    unsigned int goal_x;
    unsigned int goal_y;
    unsigned int goal_z;
};

state get_level(int level);

} }
