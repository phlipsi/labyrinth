#include <point.hpp>

#include <cmath>

namespace graphic {

point point::rotate(float alpha) const {
    const float alpha_rad = 2 * alpha * M_PI / 360.0;
    const float new_x = std::cos(alpha_rad) * x - std::sin(alpha_rad) * y;
    const float new_y = std::sin(alpha_rad) * x + std::cos(alpha_rad) * y;
    return point{ static_cast<int>(std::round(new_x)), static_cast<int>(std::round(new_y)) };
}
point point::shift(point delta) const {
    return point{ x + delta.x, y + delta.y };
}

}
