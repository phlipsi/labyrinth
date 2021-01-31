#pragma once

#include <movement.hpp>
#include <message.hpp>

#include <string_view>
#include <vector>

namespace labyrinth { namespace common {

class message;

class handler {
public:
    virtual ~handler() = default;

    message dispatch(int client_id, const message &received);

    virtual bool on_idle();

    virtual void on_ok(int client_id);
    virtual void on_error(int client_id, std::string_view text);
    virtual std::vector<char> on_unknown(int client_id, const std::vector<char> &payload);

    virtual bool on_client_hello(int client_id, std::string_view name);
    virtual void on_client_quit(int client_id);

    virtual std::vector<char> on_push_update(int client_id, const std::vector<char> &payload);
    virtual std::vector<char> on_try_push_movement(int client_id, movement m);
    virtual std::vector<char> on_get_state(int client_id);

    virtual void on_server_quit(int client_id);
    virtual void on_send_message(int client_id, std::string_view recipient, std::string_view text);
};

} }
