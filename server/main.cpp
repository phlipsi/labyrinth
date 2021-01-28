#include "handler.hpp"
#include "server_base.hpp"

#include <initializer.hpp>

#include <iostream>

class test_handler : public labyrinth::server::handler {
public:
    virtual bool idle() override {
        std::cout << "idle" << std::endl;
        return true;
    }

    virtual void server_quit(int i, const char *data, size_t data_len) override {
        std::cout << "server_quit: " << std::string(data, data_len) << std::endl;
    }

    virtual std::vector<char> send_message(int i, const char *data, size_t data_len) override {
        std::cout << "send_message: " << std::string(data, data_len) << std::endl;
        return { 'O', 'K' };
    }
};

int main(int argc, char *argv[]) {
    labyrinth::common::initializer init(0);
    labyrinth::server::server_base s(10000, 10);
    test_handler h;
    s.run(h, 1000);
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
