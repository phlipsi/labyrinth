#include <collection.hpp>

namespace graphic {

void collection::add_child(game_object &child) {
    children.push_back(&child);
}

void collection::draw(game &g, const renderer &r) const {
    for (const game_object *o : children) {
        o->draw(g, r);
    }
}

void collection::update(game &g, uint32_t elapsed_ticks) {
    for (game_object *o : children) {
        o->update(g, elapsed_ticks);
    }
}

bool collection::check_collisions(game &g, const game_object &other) const {
    bool collided = false;
    for (const game_object *o : children) {
        if (o->check_collisions(g, other)) {
            collided = true;
        }
    }
    return collided;
}

event::handling_result collection::handle_event(game &g, const event &e) {
    switch (e.get_type()) {
    case event::type::INVALIDATE_LEAF_CACHE:
        leaf_cache.clear();
        break;
    }
    for (game_object *o : children) {
        event::handling_result result = o->handle_event(g, e);
        if (result == event::handling_result::ABORT) {
            return event::handling_result::ABORT;
        }
    }
    return event::handling_result::CONTINUE;
}

std::vector<game_object *> collection::collect_leafs() {
    if (leaf_cache.empty()) {
        for (game_object *o : children) {
            const std::vector<game_object *> leafs = o->collect_leafs();
            leaf_cache.insert(leaf_cache.end(), leafs.begin(), leafs.end());
        }
    }
    return leaf_cache;
}

}
