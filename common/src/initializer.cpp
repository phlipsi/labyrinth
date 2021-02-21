#include <initializer.hpp>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_net.h>
#include <SDL_ttf.h>

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
            SDLNet_Quit();
            SDL_Log("Unable to initialize SDL2 mixer: %s", Mix_GetError());
            throw std::runtime_error("Error initializing SDL2 mixer");
        }
    }
    if ((subsystems & FONT) != 0) {
        if (TTF_Init() != 0) {
            Mix_Quit();
            SDLNet_Quit();
            SDL_Quit();
            SDL_Log("Unable to initialize SDL2 font engine: %s", TTF_GetError());
            throw std::runtime_error("Error initializing SDL2 font engine");
        }
    }
}

initializer::~initializer() {
    Mix_Quit();
    SDLNet_Quit();
    SDL_Quit();
}

} }
