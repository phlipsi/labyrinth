#include <state.hpp>

#include <payload.hpp>

#include <stdexcept>

#include <SDL_net.h>

namespace labyrinth { namespace common {

state::state()
  : state(get_level(0)) { }

state::state(std::string title,
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
             unsigned int goal_z)
  : title(title),
    level(level),
    width(width),
    height(height),
    depth(depth),
    tiles(std::move(tiles)),
    x(x),
    y(y),
    z(z),
    goal_x(goal_x),
    goal_y(goal_y),
    goal_z(goal_z)
{ }

void state::read_from(const char *&it, const char *const end) {
    title = read_string(it, end);
    level = read_uint(it, end);
    width = read_uint(it, end);
    height = read_uint(it, end);
    depth = read_uint(it, end);
    tiles = read_uint_vector(it, end, width * height *depth);
    x = read_uint(it, end);
    y = read_uint(it, end);
    z = read_uint(it, end);
    goal_x = read_uint(it, end);
    goal_y = read_uint(it, end);
    goal_z = read_uint(it, end);
}

size_t state::size() const {
    return sizeof(Uint32) * (10 + width * height * depth + title.length() + 1);
}

void state::write_to(char *&it, const char *const end) const {
    write_string(title, it, end);
    write_uint(level, it, end);
    write_uint(width, it, end);
    write_uint(height, it, end);
    write_uint(depth, it, end);
    write_uint_vector(tiles, it, end);
    write_uint(x, it, end);
    write_uint(y, it, end);
    write_uint(z, it, end);
    write_uint(goal_x, it, end);
    write_uint(goal_y, it, end);
    write_uint(goal_z, it, end);
}

unsigned int state::get_tile(unsigned int x, unsigned int y, unsigned int z) const {
    return tiles[x + width * y + width * height * z];
}


bool state::try_movement(movement m) {
    unsigned int tile = get_tile(x, y, z);
    switch (m) {
    case movement::LEFT:
        if ((tile & 0b000010) == 0) {
            --x;
            return true;
        } else {
            return false;
        }
    case movement::RIGHT:
        if ((tile & 0b000001) == 0) {
            ++x;
            return true;
        } else {
            return false;
        }
    case movement::UP:
        if ((tile & 0b001000) == 0) {
            --y;
            return true;
        } else {
            return false;
        }
    case movement::DOWN:
        if ((tile & 0b000100) == 0) {
            ++y;
            return true;
        } else {
            return false;
        }
    case movement::FORTH:
        if ((tile & 0b010000) == 0) {
            ++z;
            return true;
        } else {
            return false;
        }
    case movement::BACK:
        if ((tile & 0b100000) == 0) {
            --z;
            return true;
        } else {
            return false;
        }
    case movement::NONE:
    default:
        return true;
    }
}

bool state::win() const {
    return x == goal_x && y == goal_y && z == goal_z;
}

bool tile_has_left_wall(unsigned int tile) {
    return (tile & 0b000010) != 0;
}

bool tile_has_right_wall(unsigned int tile) {
    return (tile & 0b000001) != 0;
}

bool tile_has_top_wall(unsigned int tile) {
    return (tile & 0b001000) != 0;
}

bool tile_has_bottom_wall(unsigned int tile) {
    return (tile & 0b000100) != 0;
}

bool tile_has_front_wall(unsigned int tile) {
    return (tile & 0b100000) != 0;
}

bool tile_has_back_wall(unsigned int tile) {
    return (tile & 0b010000) != 0;
}

std::tuple<int, int, int> state::get_movement_diff(const state &other) const {
    return { other.x - x, other.y - y, other.z - z };
}


// 076543210
//    vhuolr

state get_level(int level) {
    if (level == 0) {
        return state("first come first move",
                     level,    // vhoulr    vhoulr
                     2, 1, 1, { 0b111110, 0b111101 },
                     0, 0, 0,
                     1, 0, 0);
    } else if (level == 1) {
        return state("but the other one too",
                     level,    // vhoulr    vhoulr
                     1, 1, 2, { 0b101111, 0b011111 },
                     0, 0, 0,
                     0, 0, 1);
    } else if (level == 2) {
        return state("don't see that comming",
                     level,    // vhoulr    vhoulr
                     2, 1, 2, { 0b111110,  0b101101,
                                0b1000000, 0b011111 },
                     0, 0, 0,
                     1, 0, 1);
    } else if (level == 3) {
        return state("behind a wall",
                     level,    // vhoulr    vhoulr
                     2, 1, 2, { 0b111110, 0b101101,
                                0b111110, 0b011101 },
                     0, 0, 0,
                     0, 0, 1);
    } else if (level == 4) {
        return state("it's a trap",
                     level,    // vhoulr    vhoulr
                     3, 1, 2, { 0b111110, 0b101100, 0b101101,
                               0b1000000, 0b011111, 0b011111 },
                     0, 0, 0,
                     2, 0, 1);
    } else if (level == 5) {
        return state("it's a trap",
                     level,    // vhoulr    vhoulr
                     3, 1, 2, { 0b111110, 0b101100, 0b101101,
                               0b1000000, 0b011111, 0b011111 },
                     0, 0, 0,
                     2, 0, 1);
    } else if (level == 6) {
        return state("down the well",
                     level,    // vhoulr    vhoulr
                     2, 2, 2, { 0b111110, 0b111001,
                               0b1000000, 0b100111,
                               0b1000000, 0b1000000,
                               0b1000000, 0b011111 },
                     0, 0, 0,
                     1, 1, 1);
    } else if (level == 7) {
        return state("but not always",
                     level,    // vhoulr    vhoulr
                     3, 2, 2, { 0b111110, 0b101001,  0b1000000,
                               0b1000000, 0b110110,  0b111101,
                               0b1000000, 0b011111,  0b1000000,
                               0b1000000, 0b1000000, 0b1000000 },
                     0, 0, 0,
                     1, 0, 1);
    } else if (level == 8) {
        return state("in depth",
                     level,    // vhoulr    vhoulr
                     3, 1, 3, { 0b111110, 0b101100,  0b111101,
                               0b1000000, 0b001111,  0b1000000,
                                0b111110, 0b011100,  0b111101 },
                     0, 0, 0,
                     2, 0, 2);
    } else if (level == 9) {
        return state("crossroad",
                     level,    // vhoulr    vhoulr
                     3, 3, 3, { 0b1000000, 0b1000000, 0b1000000,
                                0b1000000,  0b101111, 0b1000000,
                                0b1000000, 0b1000000, 0b1000000,

                                0b1000000,  0b111011, 0b1000000,
                                 0b111110,  0b000000,  0b111101,
                                0b1000000,  0b110111, 0b1000000,

                                0b1000000, 0b1000000, 0b1000000,
                                0b1000000,  0b011110,  0b111101,
                                0b1000000, 0b1000000, 0b1000000 },
                     0, 1, 1,
                     2, 1, 2);
    } else if (level == 10) {
        return state("back and forth",
                     level,    // vhoulr    vhoulr
                     2, 2, 2, { 0b111110, 0b101101,
                                0b111110, 0b101101,
                                0b111010, 0b011101,
                                0b110110, 0b011101 },
                     0, 0, 0,
                     0, 1, 0);
    } else {
        return state("finally breakfast!", MAX_LEVELS, 1, 1, 1, { 0b111111 }, 0, 0, 0, 0, 0, 0);
    }
}

std::vector<char> write(const state &s, int perspective) {
    std::vector<char> result(s.size() + sizeof(Uint32));
    char *it = result.data();
    const char *const end = result.data() + result.size();
    s.write_to(it, end);
    write_uint(perspective, it, end);
    return result;
}
std::pair<state, int> read(const std::vector<char> &payload) {
    const char *it = payload.data();
    const char *const end = payload.data() + payload.size();
    std::pair<state, int> result;
    result.first.read_from(it, end);
    result.second = read_uint(it, end);
    return result;
}

} }
