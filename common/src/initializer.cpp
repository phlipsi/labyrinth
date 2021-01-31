#include <initializer.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

#include <stdexcept>

namespace labyrinth { namespace common {

initializer::initializer(unsigned int subsystems) {
    Uint32 flags = 0;
    if ((subsystems & VIDEO) != 0) {
        flags |= SDL_INIT_VIDEO;
    }
    if ((subsystems & MUSIC) != 0) {
        flags |= SDL_INIT_AUDIO;
    }
    if (SDL_Init(flags) < 0) {
        SDL_Log("Unable to initialize SDL2: %s", SDL_GetError());
        throw std::runtime_error("Error initializing SDL2");
    }
    if (SDLNet_Init() < 0) {
        SDL_Quit();
        SDL_Log("Unable to initialize SDL2 networking: %s", SDLNet_GetError());
        throw std::runtime_error("Error initializing SDL2 networking");
    }
    if ((subsystems & MUSIC) != 0) {
        const int result = Mix_Init(MIX_INIT_OGG);
        if ((result & MIX_INIT_OGG) == 0) {
            SDL_Quit();
            SDL_Log("Unable to initialize SDL2 mixer: %s", Mix_GetError());
            throw std::runtime_error("Error initializing SDL2 mixer");
        }
    }
}

initializer::~initializer() {
    SDLNet_Quit();
    SDL_Quit();
}

} }
