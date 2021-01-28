#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

int main() {
    IPaddress address;
    TCPsocket server;
  int data;

    if(SDL_Init (0) < 0) {
        SDL_Log("SDL_Init: %s\n", SDL_GetError ());
        return 1;
    }
    if(SDLNet_Init () < 0) {
        SDL_Log("SDLNet_Init: %s\n", SDLNet_GetError());
        return 1;
    }
    if (SDLNet_ResolveHost (&address, "localhost", 10000) == -1) {
        SDL_Log("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        return 1;
    }
    server = SDLNet_TCP_Open(&address);
    if (server == nullptr) {
        SDL_Log("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        return 1;
    }

    std::vector<char> buffer(128);
    std::string command;
    std::string payload;
    std::string output;
    do {
        getline(std::cin, command);
        getline(std::cin, payload);
        std::string input = command + '\n' + payload;
        const int sent = SDLNet_TCP_Send(server, input.c_str(), input.length());
        if (sent < input.length()) {
            SDL_Log("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
            SDLNet_Quit();
            return 1;
        }
        const int received = SDLNet_TCP_Recv(server, &buffer[0], buffer.size());
        output.assign(&buffer[0], received);
        std::cout << output << std::endl;
    } while (output.compare(0, 11, "CLIENT_QUIT") != 0);
    SDLNet_TCP_Close(server);
}
