#pragma once

#include "command_line.hpp"
#include "event.hpp"

#include <initializer.hpp>
#include <message.hpp>

#include <SDL_events.h>
#include <SDL_net.h>

#include <cstdint>
#include <functional>
#include <map>
#include <typeinfo>
#include <typeindex>

namespace labyrinth { namespace client {

class game_object;

class game {
public:
    game(int argc, char *argv[]);
    virtual ~game();

    void run();

    virtual event::handling_result handle_event(const event &e);
    virtual void update(uint32_t elapsed_ticks) = 0;
    virtual void set_state(const std::vector<char> &payload) = 0;

    common::message send_to_server(const common::message &m) const;

    bool is_no_music() const { return parser.is_no_music(); }
    bool is_debug() const { return parser.is_debug(); }

protected:

    template<typename T, typename U, typename F>
    void register_collision_callback(F f);

    template<typename T, typename U>
    bool check_collisions(const T &a, const U &b);

private:
    command_line parser;
    common::initializer init;
    TCPsocket socket;
    bool quit;

    using collision_callback = std::function<bool (game &g, const game_object &a, const game_object &b)>;
    std::map<std::type_index, std::map<std::type_index, collision_callback>> collision_callbacks;

    void dispatch_event(const SDL_Event &e);
    void dispatch_key_event(const SDL_KeyboardEvent &e);
};

template<typename T, typename U, typename F>
void game::register_collision_callback(F f) {
    collision_callbacks[std::type_index(typeid(T))].emplace(std::type_index(typeid(U)), f);
}

template<typename T, typename U>
bool game::check_collisions(const T &a, const U &b) {
    const auto it1 = collision_callbacks.find(std::type_index(typeid(a)));
    if (it1 != collision_callbacks.end()) {
        const auto &inner = it1->second;
        const auto it2 = inner.find(std::type_index(typeid(b)));
        if (it2 != inner.end()) {
            return it2->second(*this, a, b);
        }
    }
    return false;
}

} }
