#include "game.hpp"

#include "game_object.hpp"

#include <SDL2/SDL_net.h>
#include <SDL2/SDL_timer.h>

namespace labyrinth { namespace client {

namespace {

TCPsocket open_socket(const std::string &hostname, uint16_t port) {
    IPaddress address;
    if (SDLNet_ResolveHost(&address, hostname.c_str(), port) < 0) {
        SDL_Log("Client: Error resolving host: %s", SDLNet_GetError());
        throw std::runtime_error("Error resolving host");
    }
    const TCPsocket result = SDLNet_TCP_Open(&address);
    if (result == nullptr) {
        SDL_Log("Client: Error opening socket: %s", SDLNet_GetError());
        throw std::runtime_error("Error opening socket");
    }
    return result;
}

}

game::game(int argc, char *argv[])
  : parser(argc, argv),
    init(common::initializer::VIDEO | common::initializer::NETWORKING),
    socket(open_socket(parser.get_hostname(), parser.get_port())),
    quit(false)
{
    common::send(socket, common::message(common::message::type::CLIENT_HELLO, parser.get_username().c_str()));
    common::receive(socket);
}

game::~game() {
    common::send(socket, common::message(common::message::type::CLIENT_QUIT));
    common::receive(socket);
    SDLNet_TCP_Close(socket);
}

void game::run() {
    SDL_Event e;
    uint32_t start = SDL_GetTicks();
    while (!quit) {
        common::send(socket, common::message(common::message::type::GET_STATE));
        const common::message answer = common::receive(socket);
        set_state(answer.get_payload());

        while (SDL_PollEvent(&e) != 0) {
            dispatch_event(e);
        }

        uint32_t current = SDL_GetTicks();
        update(current - start);
        start = current;
    }
}

event::handling_result game::handle_event(const event &e) {
    switch (e.get_type()) {
    case event::type::QUIT:
        quit = true;
        return event::handling_result::CONTINUE;
    case event::type::PUSH_UPDATE: {
        const push_update &p = static_cast<const push_update &>(e);
        common::send(socket, common::message(common::message::type::PUSH_UPDATE, p.get_payload()));
        const common::message answer = common::receive(socket);
        set_state(answer.get_payload());
        return event::handling_result::ABORT;
    }
    }
    return event::handling_result::CONTINUE;
}

void game::dispatch_event(const SDL_Event &e) {
    switch (e.type) {
    case SDL_QUIT:
        handle_event(event(event::type::QUIT));
        break;
    case SDL_KEYDOWN:
        dispatch_key_event(e.key);
        break;
    }
}

void game::dispatch_key_event(const SDL_KeyboardEvent &e) {
    switch (e.keysym.sym) {
    case SDLK_UP:
        handle_event(key(key::code::UP));
        break;
    case SDLK_DOWN:
        handle_event(key(key::code::DOWN));
        break;
    case SDLK_LEFT:
        handle_event(key(key::code::LEFT));
        break;
    case SDLK_RIGHT:
        handle_event(key(key::code::RIGHT));
        break;
    case SDLK_ESCAPE:
        handle_event(event(event::type::QUIT));
        break;
    }
}

} }
