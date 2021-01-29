#include "bitmap.hpp"
#include "game.hpp"
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
        t(w.get_renderer().create_texture_from_surface(labyrinth::client::load_image(ASSETS_DIRECTORY "/tunnel-2-0.png")))
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
        r.copy(t, labyrinth::client::rectangle{0, 0, 128, 128}, labyrinth::client::rectangle{0, 0, 128, 128}, 0.0, labyrinth::client::point<int>{0, 0});
        r.set_draw_color(0xff, 0, 0, 0xff);
        r.fill_rect(labyrinth::client::rectangle{10 * s.x, 10 * s.y, 10, 10});
        r.present();
    }
private:
    labyrinth::client::window w;
    labyrinth::common::state s;
    labyrinth::client::texture t;
};


int main(int argc, char *argv[]) {
    labyrinth_game g(argc, argv);
    g.run();
    return 0;
}
