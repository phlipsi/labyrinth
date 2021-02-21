#include "music.hpp"

namespace labyrinth::client {

music::music(std::filesystem::path m)
  : init(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)),
    m(Mix_LoadMUS(m.u8string().c_str())), playing(false)
{
    if (this->m == nullptr) {
        throw std::runtime_error(std::string("Can't load ") + m.string() + ": " + Mix_GetError());
    }
}

music::~music() {
    if (playing) {
        Mix_HaltMusic();
    }
    Mix_FreeMusic(m);
    Mix_CloseAudio();
}

void music::play_music() {
    playing = true;
    Mix_FadeInMusic(m, -1, 2000);
}

}
