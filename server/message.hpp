#pragma once

#include <cstddef>
#include <vector>

#include <SDL2/SDL_net.h>

namespace labyrinth { namespace server {

class message {
public:
    enum class type {
        ERROR = -1,
        UNKNOWN = 0,
        SEND_MESSAGE,
        SERVER_QUIT,
        CLIENT_QUIT
    };

    message(const char *data, size_t data_len);
    message(type t, const char *payload = nullptr, size_t payload_len = 0);

    static const char *get_type_name(type t);
    static size_t get_type_name_len(type t);

    type get_type() const {
        return t;
    }

    const std::vector<char> &get_payload() const {
        return payload;
    }

    void send(TCPsocket destination) const;

private:
    type t;
    std::vector<char> payload;
};

message receive(TCPsocket source);

} }
