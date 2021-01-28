#include "server_base.hpp"

#include "handler.hpp"
#include "message.hpp"

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
    SDL_Log("Server: Let's serve!");
    while (running || num_clients > 0) {
        int ready = SDLNet_CheckSockets(socket_set, timeout);
        if (ready < 0) {
            SDL_Log("Error checking sockets: %s", SDLNet_GetError());
            throw std::runtime_error("Error checking sockets");
        } else if (ready == 0) {
            if (!h.idle()) {
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

int server_base::check_client_socket(unsigned int i, handler &h) {
    TCPsocket &client = clients[i];
    if (SDLNet_SocketReady(client)) {
        SDL_Log("Client %d: sent some data...", i);
        const message received = receive(client);
        if (running) {
            switch (received.get_type()) {
            case message::type::ERROR:
                SDL_Log("Client %d: error receiving data: %s", i, SDLNet_GetError());
                throw std::runtime_error("Error receiving data");
            case message::type::OK:
                SDL_Log("Client %d: Got OK", i);
                h.ok(i, received);
                SDL_Log("Client %d: Return OK", i);
                message(message::type::OK).send(client);
                return 1;
            case message::type::CLIENT_HELLO:
                SDL_Log("Client %d: Got CLIENT_OK", i);
                h.client_hello(i, received);
                SDL_Log("Client %d: Return OK", i);
                message(message::type::OK).send(client);
                return 1;
            case message::type::PUSH_UPDATE:
                SDL_Log("Client %d: Got PUSH_UPDATE", i);
                h.push_update(i, received).send(client);
                return 1;
            case message::type::GET_STATE:
                SDL_Log("Client %d: Got GET_STATE", i);
                h.get_state(i, received).send(client);
                return 1;
            case message::type::SEND_MESSAGE: {
                SDL_Log("Client %d: Got SEND_MESSAGE", i);
                const message answer = h.send_message(i, received);
                SDL_Log("Client %d: Return %lu bytes", i, answer.get_payload().size());
                answer.send(client);
                return 1;
            }
            case message::type::CLIENT_QUIT:
                SDL_Log("Client %d: Got CLIENT_QUIT", i);
                h.client_quit(i, received);
                SDL_Log("Client %d: Return CLIENT_QUIT", i);
                received.send(client);
                remove_client(i);
                return 1;
            case message::type::SERVER_QUIT:
                SDL_Log("Client %d: Got SERVER_QUIT", i);
                h.server_quit(i, received);
                SDL_Log("Client %d: Return CLIENT_QUIT", i);
                message(message::type::CLIENT_QUIT).send(client);
                remove_client(i);
                running = false;
                return 1;
            case message::type::UNKNOWN:
            default:
                SDL_Log("Client %d: unknown command", i);
                message(message::type::UNKNOWN).send(client);
                return 1;
            }
        } else {
            h.client_quit(i, received);
            SDL_Log("Client %d: send CLIENT_QUIT since server shuts down", i);
            message(message::type::CLIENT_QUIT).send(client);
            remove_client(i);
            return 1;
        }
    } else {
        return 0;
    }
/*        std::vector<char> data(128, 0);
        const int received = SDLNet_TCP_Recv(client, &data[0], data.size());
        if (received < 0) {
            SDL_Log("Client %d: error receiving data: %s", i, SDLNet_GetError());
            throw std::runtime_error("Error receiving data");
        } else if (received > 0) {
            SDL_Log("Client %d: received %d bytes", i, received);
            if (running) {
                const message m(&data[0], received);
                switch (m.get_type()) {
                case message::type::SEND_MESSAGE: {
                    SDL_Log("Client %d: Got SEND_MESSAGE", i);
                    const std::vector<char> answer = h->send_message(i, m.get_payload(), m.get_payload_len());
                    SDL_Log("Client %d: Return %lu bytes", i, answer.size());
                    SDLNet_TCP_Send(client, &answer[0], answer.size());
                    break;
                }
                case message::type::SERVER_QUIT:
                    SDL_Log("Client %d: Got SERVER_QUIT", i);
                    h->server_quit(client, m.get_payload(), m.get_payload_len());
                    SDL_Log("Client %d: Return CLIENT_QUIT", i);
                    SDLNet_TCP_Send(client,
                                    message::get_type_name(message::type::CLIENT_QUIT),
                                    message::get_type_name_len(message::type::CLIENT_QUIT));
                    remove_client(i);
                    running = false;
                    break;
                case message::type::CLIENT_QUIT:
                    SDL_Log("Client %d: Got CLIENT_QUIT", i);
                    h->client_quit(client, m.get_payload(), m.get_payload_len());
                    SDL_Log("Client %d: Return CLIENT_QUIT", i);
                    SDLNet_TCP_Send(client,
                                    message::get_type_name(message::type::CLIENT_QUIT),
                                    message::get_type_name_len(message::type::CLIENT_QUIT));
                    remove_client(i);
                    break;
                }
            } else {
                SDL_Log("Client %d: client quit since server shuts down", i);
                SDLNet_TCP_Send(client, CLIENT_QUIT, sizeof(CLIENT_QUIT) - 1);
            }
        } else {
            handle_data(i, CLIENT_QUIT, sizeof(CLIENT_QUIT) - 1
        SDL_Log("Client %d disconnected", i);
        SDLNet_TCP_Send(client, SERVER_QUIT, sizeof(SERVER_QUIT) - 1);
        remove_client(i);
        return 1;
    } else {
        return 0;
    }*/
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