#include "server_base.hpp"

#include "handler.hpp"

#include <initializer.hpp>

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

void server_base::run(handler &h, unsigned int timeout) {
    while (running || num_clients > 0) {
        int ready = SDLNet_CheckSockets(socket_set, timeout);
        if (ready < 0) {
            SDL_Log("Error checking sockets: %s", SDLNet_GetError());
            throw std::runtime_error("Error checking sockets");
        } else if (ready == 0) {
            running = h.idle();
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
}

namespace {

const char SEND_MESSAGE[] = "SEND_MESSAGE";
const char SERVER_QUIT[] = "SERVER_QUIT";

int starts_with(const char *data, size_t data_len, const char *s, size_t len_s) {
    if (data_len >= len_s && strncmp(&data[0], s, len_s) == 0) {
        int offset = len_s;
        if (offset < data_len && data[offset] == '\n') {
            ++offset;
        }
        return offset;
    } else {
        return 0;
    }
}

std::optional<std::vector<char>> handle_data(int i, const char *data, size_t data_len, handler &h) {
    int offset = starts_with(data, data_len, SERVER_QUIT, sizeof(SERVER_QUIT) - 1);
    if (offset > 0) {
        SDL_Log("Client %d: received SERVER_QUIT", i);
        h.server_quit(i, data + offset, data_len - offset);
        return {};
    }

    offset = starts_with(data, data_len, SEND_MESSAGE, sizeof(SEND_MESSAGE) - 1);
    if (offset > 0) {
        SDL_Log("Client %d: received SEND_MESSAGE", i);
        return h.send_message(i, data + offset, data_len - offset);
    }

    SDL_Log("Client %d: received unknown message", i);
    return std::vector<char>();
}

}

int server_base::check_client_socket(unsigned int i, handler &h) {
    TCPsocket &client = clients[i];
    if (SDLNet_SocketReady(client)) {
        SDL_Log("Client %d: sent some data...", i);
        std::vector<char> data(128, 0);
        const int received = SDLNet_TCP_Recv(client, &data[0], data.size());
        if (received < 0) {
            SDL_Log("Client %d receive error: %s", i, SDLNet_GetError());
            throw std::runtime_error("Error receiving data");
        } else {
            // TODO: received == 0 abhandeln
            SDL_Log("Client %d: received %d bytes", i, received);
            if (running) {
                const std::optional<std::vector<char>> response = handle_data(i, &data[0], received, h);
                if (response) {
                    const std::vector<char> &output = *response;
                    SDLNet_TCP_Send(client, &output[0], output.size());
                    return 1;
                }
            }
            // TODO: SERVER_QUIT response vereinheitlichen
            SDL_Log("Client %d disconnected", i);
            SDLNet_TCP_Send(client, SERVER_QUIT, sizeof(SERVER_QUIT) - 1);
            remove_client(i);
            running = false;
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
