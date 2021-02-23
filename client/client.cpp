#include "assets.hpp"
#include "bitmap.hpp"
#include "font.hpp"
#include "game.hpp"
#include "level.hpp"
#include "music.hpp"
#include "rectangle.hpp"
#include "renderer.hpp"
#include "surface.hpp"
#include "texture.hpp"
#include "window.hpp"

#include <movement.hpp>
#include <state.hpp>
#include <config.hpp>

#include <memory>

class labyrinth_game : public labyrinth::client::game {
public:
    labyrinth_game(int argc, char *argv[])
      : game(argc, argv),
        a(argv[0]),
        m(is_no_music() ? nullptr : new labyrinth::client::music(a.get_asset_path("labyrinth.ogg"))),
        w("Labyrinth", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN),
        lvl(a, w.get_renderer())
    {
        if (m) {
            m->play_music();
        }
    }

    bool try_push_movement(labyrinth::common::movement m) {
        labyrinth::common::message msg(labyrinth::common::message::type::TRY_PUSH_MOVEMENT, write_movement(m));
        const labyrinth::common::message answer = send_to_server(msg);
        if (answer.get_type() == labyrinth::common::message::type::GET_STATE) {
            const std::vector<char> &payload = answer.get_payload();
            if (!payload.empty()) {
                set_state(payload);
                return true;
            }
        }
        return false;
    }

    virtual labyrinth::client::event::handling_result handle_event(const labyrinth::client::event &e) override {
        if (e.get_type() == labyrinth::client::event::type::KEY) {
            const labyrinth::client::key &k = static_cast<const labyrinth::client::key &>(e);
            switch (k.get_code()) {
            case labyrinth::client::key::code::UP:
                try_push_movement(labyrinth::common::movement::UP);
                return labyrinth::client::event::handling_result::ABORT;
            case labyrinth::client::key::code::DOWN:
                try_push_movement(labyrinth::common::movement::DOWN);
                return labyrinth::client::event::handling_result::ABORT;
            case labyrinth::client::key::code::LEFT:
                if (lvl.get_perspective() == 0) {
                    try_push_movement(labyrinth::common::movement::LEFT);
                } else {
                    try_push_movement(labyrinth::common::movement::BACK);
                }
                return labyrinth::client::event::handling_result::ABORT;
            case labyrinth::client::key::code::RIGHT:
                if (lvl.get_perspective() == 0) {
                    try_push_movement(labyrinth::common::movement::RIGHT);
                } else {
                    try_push_movement(labyrinth::common::movement::FORTH);
                }
                return labyrinth::client::event::handling_result::ABORT;
            }
        }
        return game::handle_event(e);
    }

    virtual void set_state(const std::vector<char> &payload) override {
        const auto r = labyrinth::common::read(payload);
        lvl.set_perspective(r.second);
        lvl.move_to_state(r.first);
    }

    virtual void update(uint32_t elapsed_ticks) override {
        const labyrinth::client::renderer &r = w.get_renderer();
        r.set_draw_color(0, 0, 0, 0xff);
        r.clear();
        lvl.update(*this, r, elapsed_ticks);
        lvl.draw(*this, r);
        r.present();
    }
private:
    labyrinth::client::assets a;
    std::unique_ptr<labyrinth::client::music> m;
    labyrinth::client::window w;
    labyrinth::client::level lvl;
};


int main(int argc, char *argv[]) {
    labyrinth_game g(argc, argv);
    g.run();
    return 0;
}
