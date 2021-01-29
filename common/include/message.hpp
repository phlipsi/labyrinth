#pragma once

#include <cstddef>
#include <string>
#include <string_view>
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


private:
    type t;
    std::vector<char> payload;
};

class ok : public message {
public:
    ok() : message(type::OK) { }
};

class client_hello : public message {
    explicit client_hello(const std::string &name)
      : message(type::CLIENT_HELLO, name.data(), name.length()) { }

    std::string_view get_name() const {
        const std::vector<char> &payload = get_payload();
        return std::string_view(payload.data(), payload.size());
    }
};

class push_update : public message {
public:
    push_update(const char *payload, size_t payload_len)
      : message(type::PUSH_UPDATE, payload, payload_len) { }
};

class get_state : public message {
public:
    get_state()
      : message(type::GET_STATE) { }
};

class send_message : public message {
    send_message(const std::string &recipient, const std::string &text);

    std::string_view get_recipient() const {
        const std::vector<char> &payload = get_payload();
        return std::string_view(payload.data(), recipient_len);
    }

    std::string_view get_text() const {
        const std::vector<char> &payload = get_payload();
        return std::string_view(payload.data() + recipient_len + 1, payload.size() - recipient_len - 1);
    }

private:
    std::string_view::size_type recipient_len;
};

class client_quit : public message {
public:
    client_quit() : message(type::CLIENT_QUIT) { }
};

class server_quit : public message {
public:
    server_quit() : message(type::SERVER_QUIT) { }
};

void send(TCPsocket destination, const message &m);
message receive(TCPsocket source);

} }
