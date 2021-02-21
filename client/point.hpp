#pragma once

#include <SDL_rect.h>

namespace labyrinth { namespace client {

template<typename T>
struct point {
    T x;
    T y;

    point rotate(float alpha) const {
        const float alpha_rad = 2 * alpha * M_PI / 360.0;
        const T new_x = static_cast<T>(std::cos(alpha_rad) * x - std::sin(alpha_rad) * y);
        const T new_y = static_cast<T>(std::sin(alpha_rad) * x + std::cos(alpha_rad) * y);
        return {new_x, new_y};
    }

    float angle() const {
        return 180.0 * std::atan2(y, x) / M_PI;
    }

    T abs_length() const {
        using std::abs;
        return abs(x) + abs(y);
    }

    point shift(point delta) const {
        return {x + delta.x, y + delta.y};
    }

    point scale(T mu) const {
        return {mu * x, mu * y};
    }

    point difference(point p) const {
        return { x - p.x, y - p.y };
    }

    SDL_Point get_sdl_point() const {
        return { static_cast<int>(x), static_cast<int>(y) };
    }
};

} }
