#include "handler.hpp"
#include "message.hpp"
#include "server_base.hpp"

#include <initializer.hpp>

#include <iostream>
#include <deque>
#include <map>
#include <string>

class test_handler : public labyrinth::server::handler {
public:
    test_handler() : s(0) { }

    virtual void client_hello(int i, const labyrinth::server::message &received) override {
        const std::string name(received.get_payload().data(), received.get_payload().size());
        players.emplace(i, name);
        std::cout << "new player: " << name << std::endl;
    }

    virtual void client_quit(int i, const labyrinth::server::message &received) override {
        const auto it = players.find(i);
        std::cout << "player quits: " << it->second << std::endl;
        players.erase(it);
    }

    virtual labyrinth::server::message push_update(int i, const labyrinth::server::message &received) override {
        const std::vector<char> &payload = received.get_payload();
        int update = SDLNet_Read32(payload.data());
        s += update;
        std::vector<char> response(sizeof(int32_t));
        SDLNet_Write32(s, response.data());
        return labyrinth::server::message(labyrinth::server::message::type::GET_STATE, response.data(), response.size());
    }

    virtual labyrinth::server::message get_state(int i, const labyrinth::server::message &received) override {
        std::vector<char> response(sizeof(int32_t));
        SDLNet_Write32(s, response.data());
        return labyrinth::server::message(labyrinth::server::message::type::GET_STATE, response.data(), response.size());
    }

private:
    std::map<int, std::string> players;
    int s;
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
