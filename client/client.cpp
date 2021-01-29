#include <initializer.hpp>

#include <SDL2/SDL_net.h>
#include <message.hpp>

#include <iostream>
#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>

TCPsocket open_socket() {
    IPaddress address;
    if (SDLNet_ResolveHost(&address, "localhost", 10000) < 0) {
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

class client {
public:
    explicit client(std::string name)
      : init(labyrinth::common::initializer::VIDEO | labyrinth::common::initializer::NETWORKING),
        server(open_socket()),
        s(0)
    {
        labyrinth::common::send(server, labyrinth::common::message(labyrinth::common::message::type::CLIENT_HELLO, "philipp"));
        labyrinth::common::receive(server);
        s = get_state();
    }

    ~client() {
        labyrinth::common::send(server, labyrinth::common::message(labyrinth::common::message::type::CLIENT_QUIT));
        labyrinth::common::receive(server);
        SDLNet_TCP_Close(server);
    }

    int push_update(int u) {
        std::vector<char> update(sizeof(Uint32));
        SDLNet_Write32(u, update.data());
        labyrinth::common::send(server, labyrinth::common::message(labyrinth::common::message::type::PUSH_UPDATE, std::move(update)));
        const labyrinth::common::message response = labyrinth::common::receive(server);
        if (response.get_type() == labyrinth::common::message::type::GET_STATE) {
            s = SDLNet_Read32(response.get_payload().data());
        }
        return s;
    }

    int get_state() {
        labyrinth::common::send(server, labyrinth::common::message(labyrinth::common::message::type::GET_STATE));
        const labyrinth::common::message response = labyrinth::common::receive(server);
        if (response.get_type() == labyrinth::common::message::type::GET_STATE) {
            s = SDLNet_Read32(response.get_payload().data());
        }
        return s;
    }

private:
    labyrinth::common::initializer init;
    TCPsocket server;
    int s;
};


int main() {
    client c("philipp");
    int u = 0;
    while (true) {
        std::cin >> u;
        if (u == -1) {
            break;
        } else {
            std::cout << "new state: " << c.push_update(u) << std::endl;
        }
    }
    return 0;
}
