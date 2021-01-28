#include "message.hpp"

#include <cstring>

namespace labyrinth { namespace server {

namespace {

const char UNKNOWN[]      = "UNKNOWN";
const char OK[]           = "OK";
const char CLIENT_HELLO[] = "CLIENT_HELLO";
const char PUSH_UPDATE[]  = "PUSH_UPDATE";
const char GET_STATE[]    = "GET_STATE";
const char SEND_MESSAGE[] = "SEND_MESSAGE";
const char SERVER_QUIT[]  = "SERVER_QUIT";
const char CLIENT_QUIT[]  = "CLIENT_QUIT";

int parse_command(const char *data,
                  size_t data_len,
                  const char *command,
                  size_t command_len)
{
    if (data_len >= command_len && strncmp(&data[0], command, command_len) == 0) {
        int offset = command_len;
        if (offset < data_len && data[offset] == '\n') {
            ++offset;
        }
        return offset;
    } else {
        return 0;
    }
}

}

message::message(const char *data, size_t data_len)
  : t(type::UNKNOWN)
{
    int offset = parse_command(data, data_len, SEND_MESSAGE, sizeof(SEND_MESSAGE) - 1);
    if (offset > 0) {
        t = type::SEND_MESSAGE;
    }

    if (t == type::UNKNOWN) {
        offset = parse_command(data, data_len, OK, sizeof(OK) - 1);
        if (offset > 0) {
            t = type::OK;
        }
    }

    if (t == type::UNKNOWN) {
        offset = parse_command(data, data_len, CLIENT_HELLO, sizeof(CLIENT_HELLO) - 1);
        if (offset > 0) {
            t = type::CLIENT_HELLO;
        }
    }

    if (t == type::UNKNOWN) {
        offset = parse_command(data, data_len, PUSH_UPDATE, sizeof(PUSH_UPDATE) - 1);
        if (offset > 0) {
            t = type::PUSH_UPDATE;
        }
    }

    if (t == type::UNKNOWN) {
        offset = parse_command(data, data_len, GET_STATE, sizeof(GET_STATE) - 1);
        if (offset > 0) {
            t = type::GET_STATE;
        }
    }

    if (t == type::UNKNOWN) {
        offset = parse_command(data, data_len, SERVER_QUIT, sizeof(SERVER_QUIT) - 1);
        if (offset > 0) {
            t = type::SERVER_QUIT;
        }
    }

    if (t == type::UNKNOWN) {
        offset = parse_command(data, data_len, CLIENT_QUIT, sizeof(CLIENT_QUIT) - 1);
        if (offset > 0) {
            t = type::CLIENT_QUIT;
        }
    }

    if (t != type::UNKNOWN) {
        payload.assign(data + offset, data + data_len - offset);
    }
}

message::message(type t, const char *payload, size_t payload_len)
  : t(t),
    payload(payload, payload + payload_len)
{ }

const char *message::get_type_name(type t) {
    switch (t) {
    case type::UNKNOWN:
        return UNKNOWN;
    case type::OK:
        return OK;
    case type::CLIENT_HELLO:
        return CLIENT_HELLO;
    case type::PUSH_UPDATE:
        return PUSH_UPDATE;
    case type::GET_STATE:
        return GET_STATE;
    case type::SEND_MESSAGE:
        return SEND_MESSAGE;
    case type::SERVER_QUIT:
        return SERVER_QUIT;
    case type::CLIENT_QUIT:
        return CLIENT_QUIT;
    default:
        return nullptr;
    }
}

size_t message::get_type_name_len(type t) {
    switch (t) {
    case type::UNKNOWN:
        return sizeof(UNKNOWN) - 1;
    case type::OK:
        return sizeof(OK) - 1;
    case type::CLIENT_HELLO:
        return sizeof(CLIENT_HELLO) - 1;
    case type::PUSH_UPDATE:
        return sizeof(PUSH_UPDATE) - 1;
    case type::GET_STATE:
        return sizeof(GET_STATE) - 1;
    case type::SEND_MESSAGE:
        return sizeof(SEND_MESSAGE) - 1;
    case type::SERVER_QUIT:
        return sizeof(SERVER_QUIT) - 1;
    case type::CLIENT_QUIT:
        return sizeof(CLIENT_QUIT) - 1;
    default:
        return 0;
    }
}

void message::send(TCPsocket destination) const {
    const char *const type_name = get_type_name(t);
    const size_t type_name_len = get_type_name_len(t);

    std::vector<char> buffer;
    buffer.reserve(type_name_len + 1 + payload.size());

    buffer.insert(buffer.end(), type_name, type_name + type_name_len);
    if (!payload.empty()) {
        buffer.push_back('\n');
        buffer.insert(buffer.end(), payload.begin(), payload.end());
    }
    SDLNet_TCP_Send(destination, buffer.data(), buffer.size());
}

message receive(TCPsocket source) {
    std::vector<char> buffer(128);
    const int received = SDLNet_TCP_Recv(source, buffer.data(), buffer.size());
    if (received < 0) {
        return message(message::type::ERROR);
    } else if (received == 0) {
        return message(message::type::CLIENT_QUIT);
    } else {
        return message(buffer.data(), buffer.size());
    }
}

} }
