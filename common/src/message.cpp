#include "message.hpp"

#include <cstring>
#include <string_view>

namespace labyrinth { namespace common {

namespace {

const char UNKNOWN[]      = "UNKNOWN";
const char OK[]           = "OK";
const char CLIENT_HELLO[] = "CLIENT_HELLO";
const char PUSH_UPDATE[]  = "PUSH_UPDATE";
const char GET_STATE[]    = "GET_STATE";
const char SEND_MESSAGE[] = "SEND_MESSAGE";
const char SERVER_QUIT[]  = "SERVER_QUIT";
const char CLIENT_QUIT[]  = "CLIENT_QUIT";

message::type translate_command_to_type(const char *command, size_t command_len) {
    if (strncmp(command, OK, command_len) == 0) {
        return message::type::OK;
    } else if (strncmp(command, CLIENT_HELLO, command_len) == 0) {
        return message::type::CLIENT_HELLO;
    } else if (strncmp(command, CLIENT_QUIT, command_len) == 0) {
        return message::type::CLIENT_QUIT;
    } else if (strncmp(command, PUSH_UPDATE, command_len) == 0) {
        return message::type::PUSH_UPDATE;
    } else if (strncmp(command, GET_STATE, command_len) == 0) {
        return message::type::GET_STATE;
    } else if (strncmp(command, SEND_MESSAGE, command_len) == 0) {
        return message::type::SEND_MESSAGE;
    } else if (strncmp(command, SERVER_QUIT, command_len) == 0) {
        return message::type::SERVER_QUIT;
    } else {
        return message::type::UNKNOWN;
    }
}

std::string_view translate_type_to_command(message::type t) {
    switch (t) {
    case message::type::OK:
        return std::string_view(OK, sizeof(OK) - 1);
    case message::type::CLIENT_HELLO:
        return std::string_view(CLIENT_HELLO, sizeof(CLIENT_HELLO) - 1);
    case message::type::CLIENT_QUIT:
        return std::string_view(CLIENT_QUIT, sizeof(CLIENT_QUIT) - 1);
    case message::type::PUSH_UPDATE:
        return std::string_view(PUSH_UPDATE, sizeof(PUSH_UPDATE) - 1);
    case message::type::GET_STATE:
        return std::string_view(GET_STATE, sizeof(GET_STATE) - 1);
    case message::type::SEND_MESSAGE:
        return std::string_view(SEND_MESSAGE, sizeof(SEND_MESSAGE) - 1);
    case message::type::SERVER_QUIT:
        return std::string_view(SERVER_QUIT, sizeof(SERVER_QUIT) - 1);
    default:
        return std::string_view(UNKNOWN, sizeof(UNKNOWN) - 1);
    }
}

}

message::message(type t, std::vector<char> payload)
  : t(t),
    payload(std::move(payload))
{ }

message::message(type t, const char *payload, size_t payload_len)
  : t(t),
    payload(payload, payload + payload_len)
{ }

message::message(type t, const char *payload)
  : t(t),
    payload(payload, payload + strlen(payload))
{ }

message::message(type t)
  : t(t)
{ }

void send(TCPsocket destination, const message &m) {
    const message::type t = m.get_type();
    const std::string_view command = translate_type_to_command(t);

    const std::vector<char> &payload = m.get_payload();
    std::vector<char> buffer;
    buffer.reserve(command.length() + 1 + payload.size());

    buffer.insert(buffer.end(), command.begin(), command.end());
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
        const char *const error = SDLNet_GetError();
        return message(message::type::ERROR, error, strlen(error));
    } else if (received == 0) {
        return message(message::type::CLIENT_QUIT);
    } else {
        const char *command = buffer.data();
        const char *sep = static_cast<const char *>(memchr(buffer.data(), '\n', received));
        size_t command_len = 0;
        const char *payload = nullptr;
        size_t payload_len = 0;
        if (sep != nullptr) {
            command_len = sep - command;
            payload = sep + 1;
            payload_len = received - command_len - 1;
        } else {
            command_len = received;
            payload = "";
            payload_len = 0;
        }
        return message(translate_command_to_type(command, command_len), payload, payload_len);
    }
}

} }
