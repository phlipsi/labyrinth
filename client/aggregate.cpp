#include <aggregate.hpp>

#include <drawable.hpp>

namespace graphic {

aggregate::aggregate(point<float> pos, float angle)
  : pos(pos),
    angle(angle)
{ }

void aggregate::add_fragment(const fragment &f) {
    fragments.push_back(&f);
}

void aggregate::draw(graphic::game &g, const renderer &r) const {
    for (const fragment *f : fragments) {
        f->thing->draw(g, r, pos.shift(f->shift.rotate(angle)), angle + f->angle);
    }
}

}
