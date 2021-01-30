#pragma once

#include <vector>

namespace labyrinth::common {

unsigned int read_uint(const char *&buffer, const char *end);

std::vector<unsigned int> read_uint_vector(const char *&buffer, const char *end, unsigned int num);

void write_uint(unsigned int value, char *&buffer, const char *end);

void write_uint_vector(const std::vector<unsigned int> &values, char *&buffer, const char *end);

}
