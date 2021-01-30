#pragma once

#include <utility>
#include <vector>

namespace labyrinth { namespace common {

struct state {
    state();
    state(unsigned int level,
          unsigned int width,
          unsigned int height,
          unsigned int depth,
          std::vector<unsigned int> tiles,
          unsigned int x,
          unsigned int y,
          unsigned int z,
          unsigned int goal_x,
          unsigned int goal_y,
          unsigned int goal_z);

    void read_from(const char *data, const char *end);

    size_t size() const;
    void write_to(char *&data, const char *end) const;

    unsigned int level;
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

std::vector<char> write(const state &s, int perspective);
std::pair<state, int> read(const std::vector<char> &payload);

} }
