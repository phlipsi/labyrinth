#include <payload.hpp>

#include <SDL_net.h>

#include <stdexcept>

namespace labyrinth::common {

unsigned int read_uint(const char *&buffer, const char *end) {
    if (buffer + sizeof(Uint32) <= end) {
        unsigned int result = SDLNet_Read32(buffer);
        buffer += sizeof(Uint32);
        return result;
    } else {
        throw std::runtime_error("Not enough data in buffer");
    }
}

std::vector<unsigned int> read_uint_vector(const char *&buffer, const char *end, unsigned int num) {
    std::vector<unsigned int> result;
    result.reserve(num);
    for (std::vector<unsigned int>::size_type i = 0; i < num; ++i) {
        result.push_back(read_uint(buffer, end));
    }
    return result;
}

void write_uint(unsigned int value, char *&buffer, const char *end) {
    if (buffer + sizeof(Uint32) <= end) {
        SDLNet_Write32(value, buffer);
        buffer += sizeof(Uint32);
    } else {
        throw std::runtime_error("Not enough data in buffer");
    }
}

void write_uint_vector(const std::vector<unsigned int> &values, char *&buffer, const char *end) {
    for (unsigned int value : values) {
        write_uint(value, buffer, end);
    }
}

std::string read_string(const char *&buffer, const char *end) {
    const size_t len = read_uint(buffer, end);
    const std::vector<unsigned int> content = read_uint_vector(buffer, end, len);
    return std::string(content.begin(), content.end());
}

void write_string(std::string_view s, char *&buffer, const char *end) {
    write_uint(s.length(), buffer, end);
    for (char c : s) {
        write_uint(c, buffer, end);
    }
}

}
