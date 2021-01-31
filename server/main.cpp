#include "handler.hpp"
#include "message.hpp"
#include "server_base.hpp"

#include <initializer.hpp>
#include <state.hpp>

#include <iostream>
#include <map>
#include <string>
#include <vector>

class player {
public:
    player(std::string name)
      : name(std::move(name)),
        party_index(-1),
        perspective(-1)
    { }

    const std::string &get_name() const { return name; }
    int get_party_index() const { return party_index; }
    int get_perspective() const { return perspective; }

    void set_party_index(int party_index) { this->party_index = party_index; }
    void set_perspective(int perspective) { this->perspective = perspective; }

private:
    std::string name;
    int party_index;
    int perspective;
};

class party {
public:
    party() : first(false), second(false) { }

    bool join(player &p) {
        if (!first) {
            p.set_perspective(0);
            first = true;
            return true;
        } else if (!second) {
            p.set_perspective(1);
            second = true;
            return true;
        } else {
            return false;
        }
    }

    void disconnect(unsigned int perspective) {
        switch (perspective) {
        case 0:
            first = false;
            break;
        case 1:
            second = false;
            break;
        }
        if (!first && !second) {
            std::cout << "all players left the party, reset to first level" << std::endl;
            s = labyrinth::common::get_level(0);
        }
    }

    labyrinth::common::state &get_state() { return s; }

private:
    bool first;
    bool second;
    labyrinth::common::state s;
};

class test_handler : public labyrinth::common::handler {
public:
    test_handler() { }

    virtual bool on_client_hello(int i, std::string_view name) override {
        player &pl = players.emplace(i, player(std::string(name.begin(), name.end()))).first->second;
        for (size_t pi = 0; pi < parties.size(); ++pi) {
            if (parties[pi].join(pl)) {
                pl.set_party_index(pi);
                std::cout << "new player " << name << " joining party " << pi << std::endl;
                return true;
            }
        }
        parties.emplace_back();
        size_t pi = parties.size() - 1;
        parties[pi].join(pl);
        pl.set_party_index(pi);
        std::cout << "new player " << name << " starts new party " << pi << std::endl;
        return true;
    }

    virtual void on_client_quit(int i) override {
        const auto it = players.find(i);
        if (it != players.end()) {
            player &pl = it->second;
            const int pi = pl.get_party_index();
            party &p = parties[pi];
            p.disconnect(pl.get_perspective());
            std::cout << "player " << it->second.get_name() << " quits party " << pi << std::endl;
            players.erase(it);
        }
    }

    virtual std::vector<char> on_push_update(int i, const std::vector<char> &payload) override {
        const auto it = players.find(i);
        if (it != players.end()) {
            player &pl = it->second;
            labyrinth::common::state &s = parties[pl.get_party_index()].get_state();
            const auto u = labyrinth::common::read(payload);
            s = u.first;
            return write(s, pl.get_perspective());
        } else {
            return {};
        }
    }

    virtual std::vector<char> on_try_push_movement(int i, labyrinth::common::movement m) override {
        const auto it = players.find(i);
        if (it != players.end()) {
            player &pl = it->second;
            labyrinth::common::state &s = parties[pl.get_party_index()].get_state();
            s.try_movement(m);
            return write(s, pl.get_perspective());
        } else {
            return {};
        }
    }

    virtual std::vector<char> on_get_state(int i) override {
        const auto it = players.find(i);
        if (it != players.end()) {
            player &pl = it->second;
            labyrinth::common::state &s = parties[it->second.get_party_index()].get_state();
            return write(s, pl.get_perspective());
        } else {
            return {};
        }
    }

private:
    std::map<int, player> players;
    std::vector<party> parties;
};

int main(int argc, char *argv[]) {
    labyrinth::common::initializer init(0);
    labyrinth::server::server_base s(10000, 10);
    test_handler h;
    s.run(h, 1000);
    return 0;
}
