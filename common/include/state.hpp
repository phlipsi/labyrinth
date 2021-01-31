#pragma once

#include <movement.hpp>

#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace labyrinth { namespace common {

const int MAX_LEVELS = 3;

struct state {
    state();
    state(std::string title,
          unsigned int level,
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

    void read_from(const char *&data, const char *end);

    size_t size() const;
    void write_to(char *&data, const char *end) const;

    unsigned int get_tile(unsigned int x, unsigned int y, unsigned int z) const;

    bool try_movement(movement m);
    bool win() const;

    std::tuple<int, int, int> get_movement_diff(const state &other) const;

    std::string title;
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

bool tile_has_left_wall(unsigned int tile);
bool tile_has_right_wall(unsigned int tile);
bool tile_has_top_wall(unsigned int tile);
bool tile_has_bottom_wall(unsigned int tile);
bool tile_has_front_wall(unsigned int tile);
bool tile_has_back_wall(unsigned int tile);

state get_level(int level);

std::vector<char> write(const state &s, int perspective);
std::pair<state, int> read(const std::vector<char> &payload);

} }
