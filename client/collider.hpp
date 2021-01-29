#pragma once

namespace graphic {

class collider {
public:
    virtual ~collider() = default;

    virtual bool collides_with(const collider &other) const = 0;
};

}
