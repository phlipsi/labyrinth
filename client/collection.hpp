#pragma once

#include <game_object.hpp>

#include <vector>

namespace graphic {

class collection : public game_object {
public:
    collection() = default;

    void add_child(game_object &child);

    virtual void draw(game &g, const renderer &r) const override;
    virtual void update(game &g, uint32_t elapsed_ticks) override;
    virtual bool check_collisions(game &g, const game_object &other) const override;
    virtual event::handling_result handle_event(game &g, const event &e) override;

    virtual std::vector<game_object *> collect_leafs() override;

private:
    std::vector<game_object *> children;
    mutable std::vector<game_object *> leaf_cache;
};


/*class collection : public game_object {
public:
    class child {
    public:
        child(const game_object *object, point delta, float beta);

        const game_object &get_game_object() const;

        void set_delta(const point &delta);
        const point &get_delta() const;

        void set_beta(float beta);
        float get_beta() const;
    private:
        const game_object *object;
        point delta;
        float beta;
    };

    void add_child(child &c);

    virtual void draw(const renderer &r, const point &p, float alpha) const override;
    virtual void update(uint32_t elapsed_ticks) override;

private:
    std::vector<child *> children;
};*/

}
