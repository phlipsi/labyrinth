#include <initializer.hpp>

#include <SDL2/SDL_net.h>

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
        send("CLIENT_HELLO\n" + name);
        s = get_state();
    }

    ~client() {
        send("CLIENT_QUIT");
        SDLNet_TCP_Close(server);
    }

    int push_update(int u) {
        std::string update(sizeof(Uint32), ' ');
        SDLNet_Write32(u, update.data());
        const std::vector<char> new_state = send("PUSH_UPDATE\n" + update);
        s = SDLNet_Read32(new_state.data() + sizeof("GET_STATE"));
        return s;
    }

    int get_state() {
        const std::vector<char> new_state = send("GET_STATE");
        s = SDLNet_Read32(new_state.data() + sizeof("GET_STATE"));
        return s;
    }

    std::vector<char> send(const std::string_view &input) {
        const int sent = SDLNet_TCP_Send(server, input.data(), input.size());
        if (sent < input.size()) {
            SDL_Log("Client: Error sending: %s", SDLNet_GetError());
            throw std::runtime_error("Error sending");
        }
        std::vector<char> output(128);
        const int received = SDLNet_TCP_Recv(server, output.data(), output.size());
        output.resize(received);
        return output;
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
