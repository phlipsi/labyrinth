#pragma once

#include <point.hpp>

namespace graphic {

class game;
class renderer;

class drawable {
public:
    virtual ~drawable() = default;

    virtual void draw(game &g, const renderer &r, const point<float> &pos, float angle) const = 0;
};

}
