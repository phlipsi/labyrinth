#pragma once

namespace labyrinth { namespace client {

class game_object;

class event {
public:
    enum class type {
        NONE,
        KEY,
        INVALIDATE_LEAF_CACHE,
        COLLISION,
        QUIT
    };

    enum class handling_result {
        ABORT,
        CONTINUE
    };

    explicit event(type t) : t(t) { }

    type get_type() const { return t; }

private:
    type t;
};

class key : public event {
public:
    enum class code {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    explicit key(code c)
      : event(event::type::KEY),
        c(c)
    { }

    code get_code() const { return c; }

private:
    code c;
};

class collision : public event {
public:
    collision(game_object &a, game_object &b)
      : event(type::COLLISION),
        a(&a),
        b(&b)
    { }

    game_object &get_a() const { return *a; }
    game_object &get_b() const { return *b; }

private:
    game_object *a;
    game_object *b;
};

} }
