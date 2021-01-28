#include "handler.hpp"

#include "message.hpp"

namespace labyrinth { namespace server {

bool handler::idle() {
    return true;
}

void handler::ok(int i, const message &received) { }

void handler::client_hello(int i, const message &received) { }

void handler::client_quit(int i, const message &received) { }

void handler::server_quit(int i, const message &received) { }

message handler::send_message(int i, const message &received) {
    return message(message::type::OK);
}

message handler::push_update(int i, const message &received) {
    return message(message::type::GET_STATE);
}

message handler::get_state(int i, const message &received) {
    return message(message::type::GET_STATE);
}

} }
