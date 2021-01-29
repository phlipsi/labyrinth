#include "server_base.hpp"

#include "message.hpp"

#include <initializer.hpp>
#include <handler.hpp>

#include <algorithm>
#include <optional>
#include <stdexcept>

namespace labyrinth { namespace server {

namespace {

TCPsocket open_server_socket(uint16_t port) {
    IPaddress addr;
    if (SDLNet_ResolveHost(&addr, NULL, 10000) < 0) {
        SDL_Log("Error resolving host: %s", SDLNet_GetError());
        throw std::runtime_error("Error resolving host");
    }
    const TCPsocket socket = SDLNet_TCP_Open(&addr);
    if (socket == nullptr) {
        SDL_Log("Error opening socket: %s", SDLNet_GetError());
        throw std::runtime_error("Error opening socket");
    }
    return socket;
}

}

server_base::server_base(uint16_t port, unsigned int max_connections)
  : socket_set(SDLNet_AllocSocketSet(max_connections)),
    server_socket(open_server_socket(port)),
    clients(max_connections, nullptr),
    num_clients(0),
    running(true)
{
    if (socket_set == nullptr) {
        SDL_Log("Error allocating socket set: %s", SDLNet_GetError());
        throw std::runtime_error("Error allocating socket set");
    }
    if (SDLNet_TCP_AddSocket(socket_set, server_socket) != 1) {
        SDL_Log("Error adding server socket to socket set: %s", SDLNet_GetError());
        throw std::runtime_error("Error adding server socket to socket set");
    }
}

server_base::~server_base() {
    for (int i = 0; i < clients.size(); ++i) {
        remove_client(i);
    }
    SDLNet_TCP_DelSocket(socket_set, server_socket);
    SDLNet_TCP_Close(server_socket);
    SDLNet_FreeSocketSet(socket_set);
}

void server_base::run(common::handler &h, unsigned int timeout) {
    SDL_Log("Server: Let's serve!");
    while (running || num_clients > 0) {
        int ready = SDLNet_CheckSockets(socket_set, timeout);
        if (ready < 0) {
            SDL_Log("Error checking sockets: %s", SDLNet_GetError());
            throw std::runtime_error("Error checking sockets");
        } else if (ready == 0) {
            if (!h.on_idle()) {
                running = false;
            }
        } else if (ready > 0) {
            ready -= check_server_socket();

            std::vector<char> data(128);
            unsigned int i = 0;
            while (i < clients.size() && ready > 0) {
                TCPsocket &client = clients[i];
                if (client != nullptr) {
                    ready -= check_client_socket(i, h);
                }
                ++i;
            }
        }
    }
    SDL_Log("Server: Goodbye!");
}

int server_base::check_client_socket(unsigned int i, common::handler &h) {
    TCPsocket &client = clients[i];
    if (SDLNet_SocketReady(client)) {
        const common::message received = common::receive(client);
        const common::message answer = h.dispatch(i, received);
        if (running) {
            send(client, answer);
            switch (received.get_type()) {
            case common::message::type::CLIENT_QUIT:
                remove_client(i);
                break;
            case common::message::type::SERVER_QUIT:
                remove_client(i);
                running = false;
                break;
            }
        } else {
            send(client, common::message(common::message::type::CLIENT_QUIT));
            remove_client(i);
        }
        return 1;
    } else {
        return 0;
    }
}

int server_base::check_server_socket() {
    if (SDLNet_SocketReady(server_socket)) {
        if (num_clients < clients.size()) {
            TCPsocket client = SDLNet_TCP_Accept(server_socket);
            if (client != nullptr) {
                if (running) {
                    add_client(client);
                } else {
                    SDLNet_TCP_Close(client);
                    SDL_Log("Can't accept more clients since server shuts down");
                }
            } else {
                SDL_Log("Error accepting client: %s", SDLNet_GetError());
            }
        } else {
            SDL_Log("Can't accept more clients, already accepted %ld", clients.size());
        }
        return 1;
    } else {
        return 0;
    }
}

void server_base::add_client(TCPsocket client) {
    const auto it = std::find(clients.begin(), clients.end(), nullptr);
    if (it != clients.end()) {
        if (SDLNet_TCP_AddSocket(socket_set, client) > 0) {
            SDL_Log("Accepting client %d", num_clients);
            *it = client;
            ++num_clients;
        } else {
            SDL_Log("Error adding client socket to socket set: %s", SDLNet_GetError());
            SDLNet_TCP_Close(client);
        }
    } else {
        SDL_Log("Can't find any free client slots");
        SDLNet_TCP_Close(client);
    }
}

void server_base::remove_client(int i) {
    TCPsocket &client = clients[i];
    if (client != nullptr) {
        SDLNet_TCP_DelSocket(socket_set, client);
        SDLNet_TCP_Close(client);
        client = nullptr;
        --num_clients;
    }
}

} }
