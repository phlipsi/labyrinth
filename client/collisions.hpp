#oragma once

#include <optional>
#include <tuple>
#include <typeindex>
#include <vector>

namespace graphic {

class game_object;

class collision_detector {
public:
    void clear_leaf_cache();

    template<typename T, typename U, typename F>
    void register_collision_callback(F f);

    std::optional<std::tuple<game_object &, game_object &>> check_collisions(game_object &scene) const;

private:
    using collision_callback = std::function<bool (game &g, const game_object &a, const game_object &b)>;
    std::map<std::type_index, std::map<std::type_index, collision_callback>> collision_callbacks;

    std::vector<game_object *> leaf_cache;

    void fill_leaf_cache(game_object &scene);
};

}
