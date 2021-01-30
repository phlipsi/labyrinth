#include "bitmap.hpp"
#include "game.hpp"
#include "level.hpp"
#include "rectangle.hpp"
#include "renderer.hpp"
#include "surface.hpp"
#include "texture.hpp"
#include "window.hpp"

#include <state.hpp>
#include <config.hpp>

class labyrinth_game : public labyrinth::client::game {
public:
    labyrinth_game(int argc, char *argv[])
      : game(argc, argv),
        w("Labyrinth", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN),
        lvl(w.get_renderer(), 0, s)
    { }

    void push_update() {
        handle_event(labyrinth::client::push_update(write(s, lvl.get_perspective())));
    }

    virtual labyrinth::client::event::handling_result handle_event(const labyrinth::client::event &e) override {
        if (e.get_type() == labyrinth::client::event::type::KEY) {
            const labyrinth::client::key &k = static_cast<const labyrinth::client::key &>(e);
            switch (k.get_code()) {
            case labyrinth::client::key::code::UP:
                if (lvl.move_up()) {
                    push_update();
                }
                return labyrinth::client::event::handling_result::ABORT;
            case labyrinth::client::key::code::DOWN:
                if (lvl.move_down()) {
                    push_update();
                }
                return labyrinth::client::event::handling_result::ABORT;
            case labyrinth::client::key::code::LEFT:
                if (lvl.move_left()) {
                    push_update();
                }
                return labyrinth::client::event::handling_result::ABORT;
            case labyrinth::client::key::code::RIGHT:
                if (lvl.move_right()) {
                    push_update();
                }
                push_update();
                return labyrinth::client::event::handling_result::ABORT;
            }
        }
        return game::handle_event(e);
    }

    virtual void set_state(const std::vector<char> &payload) override {
        const auto r = labyrinth::common::read(payload);
        s = r.first;
        lvl.set_perspective(r.second);
    }

    virtual void update(uint32_t elapsed_ticks) override {
        const labyrinth::client::renderer &r = w.get_renderer();
        r.set_draw_color(0, 0, 0, 0xff);
        r.clear();
        lvl.draw(*this, r);
        r.present();
    }
private:
    labyrinth::client::window w;
    labyrinth::common::state s;
    labyrinth::client::level lvl;
};


int main(int argc, char *argv[]) {
    labyrinth_game g(argc, argv);
    g.run();
    return 0;
}
