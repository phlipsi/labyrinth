#pragma once

#include <SDL2/SDL_mixer.h>

#include <filesystem>

namespace labyrinth::client {

class music {
public:
    music(std::filesystem::path m);
    ~music();

    void play_music();
private:
    int init;
    Mix_Music *m;
    bool playing;
};

}
