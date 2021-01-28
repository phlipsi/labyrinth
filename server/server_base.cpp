#include "server_base.hpp"

#include <initializer.hpp>

#include <algorithm>
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
    for (TCPsocket client : clients) {
        if (client != nullptr) {
            remove_client(client);
        }
    }
    SDLNet_TCP_DelSocket(socket_set, server_socket);
    SDLNet_TCP_Close(server_socket);
    SDLNet_FreeSocketSet(socket_set);
}

void server_base::run(Handler *handler, unsigned int timeout) {
    const int ready = SDLNet_CheckSockets(socket_set, timeout);
    if (ready < 0) {
        SDL_Log("Error checking sockets: %s", SDLNet_GetError());
        throw std::runtime_error("Error checking sockets");
    } else if (ready > 0) {
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
        }
        std::vector<char> data(128);
        for (int i = 0; i < clients.size(); ++i) {
            TCPsocket &client = clients[i];
            if (client != nullptr && SDLNet_SocketReady(client)) {
                SDL_Log("Client %d: sent some data...", i);
                const int received = SDLNet_TCP_Recv(client, &data[0], data.size());
                if (received < 0) {
                    SDL_Log("Client %d receive error: %s", i, SDLNet_GetError());
                    throw std::runtime_error("Error receiving data");
                } else {
                    const std::string message(&data[0], received);
                    if (received == 0 || message == "QUIT") {
                        SDL_Log("Client %d disconnected: %s", i, message.c_str());
                        remove_client(client);
                        client = nullptr;
                    } else {
                        SDL_Log("Client %d: %s", i, message.c_str());
                        if (message == "SERVER_QUIT") {
                            SDL_Log("Client %d requests server quit", i);
                            running = false;
                        }
                        if (running) {
                            SDLNet_TCP_Send(client, "OK", 2);
                        } else {
                            SDLNet_TCP_Send(client, "SERVER_QUIT", 11);
                        }
                    }
                }
            }
        }
    }
    //return running || num_clients > 0;
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

void server_base::remove_client(TCPsocket client) {
    SDLNet_TCP_DelSocket(socket_set, client);
    SDLNet_TCP_Close(client);
    --num_clients;
}

} }
