#include <state.hpp>

#include <payload.hpp>

#include <stdexcept>

#include <SDL2/SDL_net.h>

namespace labyrinth { namespace common {

state::state()
  : state(get_level(0)) { }

state::state(unsigned int level,
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
  : level(level),
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
    return sizeof(Uint32) * (10 + width * height * depth);
}

void state::write_to(char *&it, const char *const end) const {
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

// 076543210
//    vhuolr

state get_level(int level) {
    if (level == 0) {
        return state(0,        // vhoulr    vhoulr
                     2, 2, 2, { 0b111110, 0b101101,
                                0b111110, 0b101101,
                                0b111010, 0b011101,
                                0b110110, 0b011101 },
                     0, 0, 0,
                     0, 1, 0);
    } else {
        throw std::runtime_error("Invalid level");
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
