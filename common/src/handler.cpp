#include "handler.hpp"

#include "message.hpp"

#include <algorithm>
#include <string>

namespace labyrinth { namespace common {

message handler::dispatch(int client_id, const message &received) {
    switch (received.get_type()) {
    case message::type::OK:
        on_ok(client_id);
        return message(message::type::OK);
    case message::type::CLIENT_HELLO: {
        const std::vector<char> &payload = received.get_payload();
        if (on_client_hello(client_id, std::string_view(payload.data(), payload.size()))) {
            return message(message::type::OK);
        } else {
            return message(message::type::ERROR, "Unable to register new client");
        }
    }
    case message::type::CLIENT_QUIT:
        on_client_quit(client_id);
        return message(message::type::CLIENT_QUIT);
    case message::type::PUSH_UPDATE:
        return message(message::type::GET_STATE, on_push_update(client_id, received.get_payload()));
    case message::type::GET_STATE:
        return message(message::type::GET_STATE, on_get_state(client_id));
    case message::type::SEND_MESSAGE: {
        const std::vector<char> &payload = received.get_payload();
        const auto it = std::find(payload.begin(), payload.end(), ';');
        if (it != payload.end()) {
            const auto recipient_len = std::distance(it, payload.begin());
            on_send_message(client_id, std::string_view(payload.data(), recipient_len),
                            std::string_view(payload.data() + recipient_len + 1, payload.size() - recipient_len - 1));
        } else {
            on_send_message(client_id, std::string_view(), std::string_view(payload.data(), payload.size()));
        }
        return message(message::type::OK);
    }
    case message::type::SERVER_QUIT:
        on_server_quit(client_id);
        return message(message::type::CLIENT_QUIT);
    default:
        return message(message::type::UNKNOWN, on_unknown(client_id, received.get_payload()));
    }
}

bool handler::on_idle() {
    return true;
}

void handler::on_ok(int client_id) { }

void handler::on_error(int client_id, std::string_view text) {
    std::string text2(text.begin(), text.end());
    SDL_Log("Client %d, error: %s", client_id, text2.c_str());
}

std::vector<char> handler::on_unknown(int client_id, const std::vector<char> &payload) {
    SDL_Log("Client %d, unknown command of %lu bytes", client_id, payload.size());
    return {};
}

bool handler::on_client_hello(int client_id, std::string_view name) {
    return true;
}

void handler::on_client_quit(int client_id) { }

std::vector<char> handler::on_push_update(int client_id, const std::vector<char> &payload) {
    return {};
}

std::vector<char> handler::on_get_state(int client_id) {
    return {};
}

void handler::on_server_quit(int client_id) { }

void handler::on_send_message(int client_id, std::string_view recipient, std::string_view text) { }

} }
