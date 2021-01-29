#pragma one

#include <game_object.hpp>
#include <point.hpp>

#include <vector>

namespace graphic {

class drawable;
class renderer;

class aggregate : public game_object {
public:
    struct fragment {
        fragment(const drawable *thing, point<float> shift, float angle)
          : thing(thing), shift(shift), angle(angle) { }

        const drawable *thing;
        point<float> shift;
        float angle;
    };

    aggregate(point<float> pos, float angle);

    void add_fragment(const fragment &f);

    template<size_t N>
    void add_fragments(fragment (&f)[N]) {
        for (size_t i = 0; i < N; ++i) {
            add_fragment(f[i]);
        }
    }

    virtual void draw(graphic::game &g, const renderer &r) const override;

protected:
    point<float> pos;
    float angle;

private:
    std::vector<const fragment *> fragments;
};

}
