cmake_minimum_required(VERSION 3.10)
project(labyrinth CXX)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_image REQUIRED IMPORTED_TARGET SDL2_image)
pkg_check_modules(SDL2_net REQUIRED IMPORTED_TARGET SDL2_net)

set(ASSETS_DIRECTORY "${CMAKE_SOURCE_DIR}/assets/" CACHE PATH "Directory for all game assets")

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)
