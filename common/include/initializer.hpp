#pragma once

#include <stdexcept>
#include <string>

namespace labyrinth { namespace common {

template<typename QuitFunc = void (*)()>
class initializer {
public:
    template<typename InitFunc, typename ErrorFunc>
    initializer(InitFunc init, QuitFunc quit, ErrorFunc error)
      : quit(quit)
    {
        if (!init()) {
            throw std::runtime_error(std::string("Error initializing: ") + error());
        }
    }
    ~initializer() {
        quit();
    }
private:
    QuitFunc quit;
};

} }
