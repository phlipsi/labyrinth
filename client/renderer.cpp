#include <renderer.hpp>

#include <rectangle.hpp>

namespace labyrinth { namespace client {

void renderer::draw_rect(const rectangle &r, float angle, const point<int> &mid) const {
    const point<int> mid2{ mid.x + r.x, mid.y + r.y };
    point p1 = point<int>{r.x, r.y}.difference(mid2);
    point p2 = point<int>{r.x + r.w, r.y}.difference(mid2);
    point p3 = point<int>{r.x + r.w, r.y + r.h}.difference(mid2);
    point p4 = point<int>{r.x, r.y + r.h}.difference(mid2);
    p1 = p1.rotate(angle).shift(mid2);
    p2 = p2.rotate(angle).shift(mid2);
    p3 = p3.rotate(angle).shift(mid2);
    p4 = p4.rotate(angle).shift(mid2);
    line(p1, p2);
    line(p2, p3);
    line(p3, p4);
    line(p4, p1);
}

} }
