#include <sprite.hpp>

#include <game.hpp>
#include <renderer.hpp>

namespace graphic {

sprite::sprite(const texture &source, rectangle source_clip, point<int> mid)
  : source(source),
    source_clip(std::move(source_clip)),
    mid(std::move(mid))
{ }

void sprite::draw(graphic::game &g, const renderer &r, const point<float> &p, float alpha) const {
    const rectangle dst{ static_cast<int>(p.x - mid.x),
                         static_cast<int>(p.y - mid.y),
                         source_clip.w,
                         source_clip.h };
    r.copy(source, source_clip, dst, alpha, mid);
    if (g.is_debug()) {
        r.set_draw_color(0xff, 0xff, 0xff, 0xff);
        r.draw_rect(dst, alpha, mid);
    }
}

}
