cmake_minimum_required(VERSION 3.10)
project(labyrinth CXX)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(SDL2)

option(ONLY_SERVER "Builds only server components" OFF)

if(UNIX)
    include(FindPkgConfig)
    pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
    pkg_check_modules(SDL2_net REQUIRED IMPORTED_TARGET SDL2_net)
    if(NOT ONLY_SERVER)
        pkg_check_modules(SDL2_image REQUIRED IMPORTED_TARGET SDL2_image)
        pkg_check_modules(SDL2_mixer REQUIRED IMPORTED_TARGET SDL2_mixer)
        pkg_check_modules(SDL2_ttf REQUIRED IMPORTED_TARGET SDL2_ttf)
    endif()
else()
    set(SDL2_ROOT "${CMAKE_SOURCE_DIR}" CACHE PATH "Root directory to SDL2 development packages")
    set(SDL2_NET_ROOT "${CMAKE_SOURCE_DIR}" CACHE PATH "Root directory to SDL2_net development packages")
    if(NOT ONLY_SERVER)
        set(SDL2_IMAGE_ROOT "${CMAKE_SOURCE_DIR}" CACHE PATH "Root directory to SDL2_image development packages")
        set(SDL2_TTF_ROOT "${CMAKE_SOURCE_DIR}" CACHE PATH "Root directory to SDL2_ttf development packages")
        set(SDL2_MIXER_ROOT "${CMAKE_SOURCE_DIR}" CACHE PATH "Root directory to SDL2_mixer development packages")
    endif()

    add_sdl_library(SDL2 ${SDL2_ROOT}
        SDL2.dll)
    add_sdl_library(SDL2main ${SDL2_ROOT})
    add_sdl_library(SDL2_net ${SDL2_NET_ROOT}
        SDL2_net.dll)
    if(NOT ONLY_SERVER)
        add_sdl_library(SDL2_ttf ${SDL2_TTF_ROOT}
            libfreetype-6.dll
            SDL2_ttf.dll
            zlib1.dll
            LICENSE.freetype.txt
            LICENSE.zlib.txt)
        add_sdl_library(SDL2_mixer ${SDL2_MIXER_ROOT}
            libogg-0.dll
            libvorbis-0.dll
            libvorbisfile-3.dll
            SDL2_mixer.dll
            LICENSE.ogg-vorbis.txt)
        add_sdl_library(SDL2_image ${SDL2_IMAGE_ROOT}
            libpng16-16.dll
            SDL2_image.dll
            LICENSE.png.txt)
    endif()
    install(FILES "${SDL2_ROOT}/COPYING.txt" DESTINATION bin RENAME "COPYING.sdl2.txt")
    install(FILES "${SDL2_NET_ROOT}/COPYING.txt" DESTINATION bin RENAME "COPYING.sdl2_net.txt")
    if(NOT ONLY_SERVER)
        install(FILES "${SDL2_TTF_ROOT}/COPYING.txt" DESTINATION bin RENAME "COPYING.sdl2_ttf.txt")
        install(FILES "${SDL2_MIXER_ROOT}/COPYING.txt" DESTINATION bin RENAME "COPYING.sdl2_mixer.txt")
        install(FILES "${SDL2_IMAGE_ROOT}/COPYING.txt" DESTINATION bin RENAME "COPYING.sdl2_image.txt")
    endif()
endif()

set(ASSETS_DIRECTORY "${CMAKE_SOURCE_DIR}/assets" CACHE PATH "Directory for game assets")

add_subdirectory(common)
add_subdirectory(server)
if(NOT ONLY_SERVER)
    add_subdirectory(client)
endif()

install(FILES LICENSE DESTINATION bin)
