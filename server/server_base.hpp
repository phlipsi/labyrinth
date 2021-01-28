#pragma once

#include <SDL2/SDL_net.h>

#include <cstdint>
#include <vector>

namespace labyrinth { namespace server {

class handler;

class server_base {
public:
    server_base(uint16_t port, unsigned int max_connections);
    ~server_base();

    void run(handler &h, unsigned int timeout);

private:
    SDLNet_SocketSet socket_set;
    std::vector<TCPsocket> clients;
    unsigned int num_clients;
    bool running;
    TCPsocket server_socket;

    int check_server_socket();
    int check_client_socket(unsigned int i, handler &h);
    void add_client(TCPsocket client);
    void remove_client(int i);
};


} }