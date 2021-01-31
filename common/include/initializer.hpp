#pragma once

#include <cstdint>

namespace labyrinth { namespace common {
    
class initializer {
public:
    static const unsigned int VIDEO      = 1 << 0;
    static const unsigned int NETWORKING = 1 << 1;
    static const unsigned int MUSIC      = 1 << 2;

    explicit initializer(unsigned int subsystems);
    ~initializer();
};

} }
