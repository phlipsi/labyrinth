#include "handler.hpp"
#include "message.hpp"
#include "server_base.hpp"

#include <initializer.hpp>

#include <iostream>
#include <map>
#include <string>

class test_handler : public labyrinth::common::handler {
public:
    test_handler() : x(10), y(10) { }

    virtual bool on_client_hello(int i, std::string_view name) override {
        players.emplace(i, std::string(name.begin(), name.end()));
        std::cout << "new player: " << name << std::endl;
        return true;
    }

    virtual void on_client_quit(int i) override {
        const auto it = players.find(i);
        std::cout << "player quits: " << it->second << std::endl;
        players.erase(it);
    }

    virtual std::vector<char> on_push_update(int i, const std::vector<char> &payload) override {
        x = SDLNet_Read32(payload.data());
        y = SDLNet_Read32(payload.data() + sizeof(Uint32));
        return on_get_state(i);
    }

    virtual std::vector<char> on_get_state(int i) override {
        std::vector<char> response(2 * sizeof(Uint32));
        SDLNet_Write32(x, response.data());
        SDLNet_Write32(y, response.data() + sizeof(Uint32));
        return response;
    }

private:
    std::map<int, std::string> players;
    int x;
    int y;
};

int main(int argc, char *argv[]) {
    labyrinth::common::initializer init(0);
    labyrinth::server::server_base s(10000, 10);
    test_handler h;
    s.run(h, 1000);
    return 0;
}
