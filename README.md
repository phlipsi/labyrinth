[![LICENSE](https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png)](http://creativecommons.org/licenses/by-nc-sa/4.0/)

Labyrinth
=========

This is a game written in C++ using SDL2 during the Global Game Jam 2021.
Summary from Global Game Jam project page:

> Two dimensional labyrinths are very easy. But unfortunately our little
> friend has to find its breakfast in a three dimensional labyrinth. This
> can't be solved alone! So fetch your best buddy and navigate this cute
> animal together to its meal.

Build
-----

In order to build this project you need

 - [CMake](ihttps://cmake.org/)
 - [SDL2](https://www.libsdl.org/)
 - [SDL2_net](https://github.com/libsdl-org/SDL_net)
 - [SDL2_image](https://github.com/libsdl-org/SDL_image)
 - [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer)
 - [SDL2_ttf](https://wiki.libsdl.org/SDL_ttf/FrontPage)

Unfortunately the whole build process is very GNU/Linux centric. With
properly installed pkg-config files the build simply is

    > mkdir build && cd build
    > cmake -DCMAKE_INSTALL_PREFIX=<Destination> ..
    > make
    > make install

You need to start the game server in order to play the game:

    > ./server

Player now join a game on the server by starting

    > ./client [--no-music] [--server=<HOSTNAME>[:<PORT>]] [--username=<USERNAME>]

Attributions
------------

 * Fabulous cave tileset "[Pixel Fantasy Caves](https://szadiart.itch.io/pixel-fantasy-caves)" by Szadi art. Thank you very much!
 * Fabulous bat gif by Jagvinder Singh, described in [this](https://youtu.be/8dIxDs407rk) video. Thank you very much!
 * [Avara](https://www.cufonfonts.com/font/avara) true type font, free for personal use
 * And last but not least: Music by my son Anton. Thank you very much!

Cover
-----

![Cover](/assets/cover.png?raw=true)
