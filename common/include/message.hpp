#pragma once

#include <cstddef>
#include <vector>

#include <SDL2/SDL_net.h>

namespace labyrinth { namespace common {

class message {
public:
    enum class type {
        ERROR = -1,
        UNKNOWN = 0,
        OK,
        CLIENT_HELLO,
        PUSH_UPDATE,
        GET_STATE,
        SEND_MESSAGE,
        SERVER_QUIT,
        CLIENT_QUIT
    };

    message(type t, std::vector<char> &&payload);
    message(type t, const char *payload, size_t payload_len);
    message(type t, const char *payload);
    message(type t);

    type get_type() const {
        return t;
    }

    const std::vector<char> &get_payload() const {
        return payload;
    }

private:
    type t;
    std::vector<char> payload;
};

void send(TCPsocket destination, const message &m);
message receive(TCPsocket source);

} }
