#pragma once

#include <cstdint>
#include <string>

namespace labyrinth { namespace client {

class command_line {
public:
    command_line(int argc, char *argv[]);

    bool is_debug() const { return debug; }
    const std::string &get_hostname() const { return hostname; }
    uint16_t get_port() const { return port; }
    const std::string &get_username() const { return username; }

private:
    bool debug;
    std::string hostname;
    std::string username;
    uint16_t port;
};

} }
