#pragma once

#include <SDL2/SDL_net.h>

#include <cstdint>
#include <vector>

namespace labyrinth { namespace server {

class Handler;

class server_base {
public:
    server_base(uint16_t port, unsigned int max_connections);
    ~server_base();

    void run(Handler *handler, unsigned int timeout);

private:
    SDLNet_SocketSet socket_set;
    std::vector<TCPsocket> clients;
    unsigned int num_clients;
    bool running;
    TCPsocket server_socket;

    void add_client(TCPsocket client);
    void remove_client(TCPsocket client);
};


} }
