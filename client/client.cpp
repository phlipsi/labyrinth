#include "game.hpp"
#include "rectangle.hpp"
#include "renderer.hpp"
#include "window.hpp"

class labyrinth_game : public labyrinth::client::game {
public:
    labyrinth_game(int argc, char *argv[])
      : game(argc, argv),
        w("Labyrinth", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN),
        x(10), y(10)
    { }

    void push_update() {
        std::vector<char> payload(2 * sizeof(Uint32));
        SDLNet_Write32(x, payload.data());
        SDLNet_Write32(y, payload.data() + sizeof(Uint32));
        handle_event(labyrinth::client::push_update(std::move(payload)));
    }

    virtual labyrinth::client::event::handling_result handle_event(const labyrinth::client::event &e) override {
        if (e.get_type() == labyrinth::client::event::type::KEY) {
            const labyrinth::client::key &k = static_cast<const labyrinth::client::key &>(e);
            switch (k.get_code()) {
            case labyrinth::client::key::code::UP:
                y -= 5;
                push_update();
                return labyrinth::client::event::handling_result::ABORT;
            case labyrinth::client::key::code::DOWN:
                y += 5;
                push_update();
                return labyrinth::client::event::handling_result::ABORT;
            case labyrinth::client::key::code::LEFT:
                x -= 5;
                push_update();
                return labyrinth::client::event::handling_result::ABORT;
            case labyrinth::client::key::code::RIGHT:
                x += 5;
                push_update();
                return labyrinth::client::event::handling_result::ABORT;
            }
        }
        return game::handle_event(e);
    }

    virtual void set_state(const std::vector<char> &payload) override {
        x = SDLNet_Read32(payload.data());
        y = SDLNet_Read32(payload.data() + sizeof(Uint32));
    }

    virtual void update(uint32_t elapsed_ticks) override {
        const labyrinth::client::renderer &r = w.get_renderer();
        r.set_draw_color(0, 0, 0, 0xff);
        r.clear();
        r.set_draw_color(0xff, 0, 0, 0xff);
        r.fill_rect(labyrinth::client::rectangle{x, y, 10, 10});
        r.present();
    }
private:
    labyrinth::client::window w;
    int x;
    int y;
};


int main(int argc, char *argv[]) {
    labyrinth_game g(argc, argv);
    g.run();
    return 0;
}
