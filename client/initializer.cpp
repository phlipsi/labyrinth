#include <initializer.hpp>

#include <SDL2/SDL.h>

#include <stdexcept>

namespace graphic {

initializer::initializer() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("Error initializing SDL2");
    }
}

initializer::~initializer() {
    SDL_Quit();
}

}
