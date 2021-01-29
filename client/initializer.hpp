#pragma once

namespace graphic {

class initializer {
public:
    initializer();
    ~initializer();

    initializer(const initializer &) = delete;
    initializer &operator = (const initializer &) = delete;
};

}
