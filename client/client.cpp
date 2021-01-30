#include "bitmap.hpp"
#include "game.hpp"
#include "rectangle.hpp"
#include "renderer.hpp"
#include "surface.hpp"
#include "texture.hpp"
#include "walls.hpp"
#include "window.hpp"

#include <state.hpp>
#include <config.hpp>

class labyrinth_game : public labyrinth::client::game {
public:
    labyrinth_game(int argc, char *argv[])
      : game(argc, argv),
        w("Labyrinth", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN),
        ws(w.get_renderer(), 1, s)
    { }

    void push_update() {
        handle_event(labyrinth::client::push_update(s.write_to()));
    }

    virtual labyrinth::client::event::handling_result handle_event(const labyrinth::client::event &e) override {
        if (e.get_type() == labyrinth::client::event::type::KEY) {
            const labyrinth::client::key &k = static_cast<const labyrinth::client::key &>(e);
            switch (k.get_code()) {
            case labyrinth::client::key::code::UP:
                --s.y;
                push_update();
                return labyrinth::client::event::handling_result::ABORT;
            case labyrinth::client::key::code::DOWN:
                ++s.y;
                push_update();
                return labyrinth::client::event::handling_result::ABORT;
            case labyrinth::client::key::code::LEFT:
                --s.x;
                push_update();
                return labyrinth::client::event::handling_result::ABORT;
            case labyrinth::client::key::code::RIGHT:
                ++s.x;
                push_update();
                return labyrinth::client::event::handling_result::ABORT;
            }
        }
        return game::handle_event(e);
    }

    virtual void set_state(const std::vector<char> &payload) override {
        s.read_from(payload);
    }

    virtual void update(uint32_t elapsed_ticks) override {
        const labyrinth::client::renderer &r = w.get_renderer();
        r.set_draw_color(0, 0, 0, 0xff);
        r.clear();
        ws.draw(*this, r);
        r.present();
    }
private:
    labyrinth::client::window w;
    labyrinth::common::state s;
    labyrinth::client::walls ws;
};


int main(int argc, char *argv[]) {
    labyrinth_game g(argc, argv);
    g.run();
    return 0;
}
