#pragma once

#include <cstddef>
#include <vector>

namespace labyrinth { namespace common {

class state {
public:
    state(const char *buffer, size_t max);

private:
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

} }
