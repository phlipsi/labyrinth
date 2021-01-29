#pragma once

#include <drawable.hpp>
#include <point.hpp>
#include <rectangle.hpp>

namespace graphic {

class game;
class renderer;
class texture;

class sprite : public drawable {
public:
    sprite(const texture &source, rectangle source_rect, point<int> mid);

    void draw(game &g, const renderer &r, const point<float> &p, float alpha) const;

private:
    const texture &source;
    const rectangle source_clip;
    const point<int> mid;
};

}
