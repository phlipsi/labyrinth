#include <initializer.hpp>

#include <SDL2/SDL_net.h>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

class server {
public:
    server(unsigned int max_connections)
      : socket_set(SDLNet_AllocSocketSet(max_connections)),
        clients(max_connections, nullptr),
        num_clients(0),
        running(true)
    {
        if (socket_set == nullptr) {
            SDL_Log("Error allocating socket set: %s", SDLNet_GetError());
            throw std::runtime_error("Error allocating socket set");
        }

        IPaddress addr;
        if (SDLNet_ResolveHost(&addr, NULL, 10000) < 0) {
            SDL_Log("Error resolving host: %s", SDLNet_GetError());
            throw std::runtime_error("Error resolving host");
        }
        server_socket = SDLNet_TCP_Open(&addr);
        if (server_socket == nullptr) {
            SDL_Log("Error opening socket: %s", SDLNet_GetError());
            throw std::runtime_error("Error opening socket");
        }
        if (SDLNet_TCP_AddSocket(socket_set, server_socket) != 1) {
            SDL_Log("Error adding server socket to socket set: %s", SDLNet_GetError());
            throw std::runtime_error("Error adding server socket to socket set");
        }
    }

    ~server() {
        for (TCPsocket client : clients) {
            if (client != nullptr) {
                remove_client(client);
            }
        }
        SDLNet_TCP_DelSocket(socket_set, server_socket);
        SDLNet_TCP_Close(server_socket);
        SDLNet_FreeSocketSet(socket_set);
    }

    bool process(unsigned int timeout) {
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
        return running || num_clients > 0;
    }

private:
    SDLNet_SocketSet socket_set;
    std::vector<TCPsocket> clients;
    unsigned int num_clients;
    bool running;
    TCPsocket server_socket;

    void add_client(TCPsocket client) {
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

    void remove_client(TCPsocket client) {
        SDLNet_TCP_DelSocket(socket_set, client);
        SDLNet_TCP_Close(client);
        --num_clients;
    }

};

int main(int argc, char *argv[]) {
    labyrinth::common::initializer init(0);

    server s(10);
    while (s.process(1000)) {
        SDL_Log("Waiting...");
    }
    return 0;
/*
    IPaddress addr;
    if (SDLNet_ResolveHost(&addr, NULL, 10000) < 0) {
        SDL_Log("Error resolving host: %s", SDLNet_GetError());
        return 1;
    }
    TCPsocket server = SDLNet_TCP_Open(&addr);
    if (server == nullptr) {
        SDL_Log("Error opening socket: %s", SDLNet_GetError());
        return 1;
    }

    const int MAX_CLIENTS = 2;
    int connected = 0;
    TCPsocket clients[MAX_CLIENTS] = { nullptr };
    while (connected < MAX_CLIENTS) {
        SDL_Log("Waiting for players...");
        TCPsocket client = SDLNet_TCP_Accept(server);
        if (client != nullptr) {
            SDL_Log("Accepted client %d", connected + 1);
            clients[connected] = client;
            ++connected;
        }
        SDL_Delay(1000);
    }
    SDLNet_SocketSet set = SDLNet_AllocSocketSet(MAX_CLIENTS);
    if (set == nullptr) {
        SDL_Log("Error allocating socket set: %s", SDLNet_GetError());
        return 1;
    }

    SDLNet_TCP_AddSocket(set, clients[0]);
    if (SDLNet_TCP_AddSocket(set, clients[1]) != 2) {
        SDL_Log("Error adding sockets to socket set: %s", SDLNet_GetError());
        return 1;
    }

    std::vector<char> data(128);
    while (connected > 0) {
        const int ready = SDLNet_CheckSockets(set, 1000);
        if (ready < 0) {
            SDL_Log("Error checking sockets in socket set: %s", SDLNet_GetError());
            return 1;
        } else if (ready == 0) {
            SDL_Log("Nothing received yet");
        } else {
            for (size_t i = 0; i < MAX_CLIENTS; ++i) {
                if (clients[i] != nullptr && SDLNet_SocketReady(clients[i])) {
                    SDL_Log("Client %ld: sent some data...", i);
                    const int received = SDLNet_TCP_Recv(clients[i], &data[0], data.size());
                    if (received < 0) {
                        SDL_Log("Client %ld error: %s", i, SDLNet_GetError());
                        return 1;
                    } else {
                        const std::string message(&data[0], received);
                        if (received == 0 || message == "QUIT") {
                            SDL_Log("Client %ld disconnected: %s", i, message.c_str());
                            SDLNet_TCP_DelSocket(set, clients[i]);
                            SDLNet_TCP_Close(clients[i]);
                            clients[i] = nullptr;
                            --connected;
                        } else {
                            SDL_Log("Client %ld: %s", i, message.c_str());
                            SDLNet_TCP_Send(clients[i], "OK", 2);
                        }
                    }
                }
            }
        }
    }

    SDLNet_FreeSocketSet(set);
    SDLNet_TCP_Close(server);*/
    return 0;
}
