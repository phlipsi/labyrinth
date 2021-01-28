#pragma once

#include <cstddef>
#include <vector>

namespace labyrinth { namespace server {

class handler {
public:
    virtual ~handler() = default;

    virtual bool idle() = 0;
    virtual void server_quit(int i, const char *data, size_t data_len) = 0;
    virtual std::vector<char> send_message(int i, const char *data, size_t data_len) = 0;
};

} }
