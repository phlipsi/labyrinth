#include <state.hpp>

#include <stdexcept>

#include <SDL2/SDL_net.h>

namespace labyrinth { namespace common {

namespace {

unsigned int read(const char *&buffer, const char *end) {
    if (buffer + sizeof(Uint32) <= end) {
        unsigned int result = SDLNet_Read32(buffer);
        buffer += sizeof(Uint32);
        return result;
    } else {
        throw std::runtime_error("Not enough data in buffer");
    }
}

std::vector<unsigned int> read_vector(const char *&buffer, const char *end, unsigned int num) {
    std::vector<unsigned int> result;
    result.reserve(num);
    for (std::vector<unsigned int>::size_type i = 0; i < num; ++i) {
        result.push_back(read(buffer, end));
    }
    return result;
}

void write(unsigned int value, char *&buffer, const char *end) {
    if (buffer + sizeof(Uint32) <= end) {
        SDLNet_Write32(value, buffer);
        buffer += sizeof(Uint32);
    } else {
        throw std::runtime_error("Not enough data in buffer");
    }
}

void write_vector(const std::vector<unsigned int> &values, char *&buffer, const char *end) {
    for (unsigned int value : values) {
        write(value, buffer, end);
    }
}

}

state::state()
  : width(0),
    height(0),
    depth(0),
    x(0),
    y(0),
    z(0),
    goal_x(0),
    goal_y(0),
    goal_z(0)
{ }

state::state(unsigned int width,
             unsigned int height,
             unsigned int depth,
             std::vector<unsigned int> tiles,
             unsigned int x,
             unsigned int y,
             unsigned int z,
             unsigned int goal_x,
             unsigned int goal_y,
             unsigned int goal_z)
  : width(width),
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

void state::read_from(const std::vector<char> &payload) {
    const char *it = payload.data();
    const char *const end = payload.data() + payload.size();
    width = read(it, end);
    height = read(it, end);
    depth = read(it, end);
    tiles = read_vector(it, end, width * height *depth);
    x = read(it, end);
    y = read(it, end);
    z = read(it, end);
    goal_x = read(it, end);
    goal_y = read(it, end);
    goal_z = read(it, end);
}

std::vector<char> state::write_to() const {
    std::vector<char> result(sizeof(Uint32) * (9 + width * height * depth));
    char *it = result.data();
    const char *const end = result.data() + result.size();
    write(width, it, end);
    write(height, it, end);
    write(depth, it, end);
    write_vector(tiles, it, end);
    write(x, it, end);
    write(y, it, end);
    write(z, it, end);
    write(goal_x, it, end);
    write(goal_y, it, end);
    write(goal_z, it, end);
    return result;
}

// 076543210
//    hvoulr

state get_level(int level) {
    if (level == 0) {
        return state(2, 2, 2, { 0b111110, 0b011101,
                                0b111110, 0b011101,
                                0b111010, 0b101101,
                                0b110110, 0b101101 },
                     0, 0, 0,
                     1, 0, 0);
    } else {
        throw std::runtime_error("Invalid level");
    }
}

} }
