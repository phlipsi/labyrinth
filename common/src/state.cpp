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

}

state::state(const char *buffer, size_t max)
  : width(read(buffer, buffer + max)),
    height(read(buffer, buffer + max)),
    depth(read(buffer, buffer + max)),
    tiles(read_vector(buffer, buffer + max, width * height * depth)),
    x(read(buffer, buffer + max)),
    y(read(buffer, buffer + max)),
    z(read(buffer, buffer + max)),
    goal_x(read(buffer, buffer + max)),
    goal_y(read(buffer, buffer + max)),
    goal_z(read(buffer, buffer + max))
{ }

} }
