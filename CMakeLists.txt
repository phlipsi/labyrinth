cmake_minimum_required(VERSION 3.10)
project(labyrinth CXX)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_image REQUIRED IMPORTED_TARGET SDL2_image)
pkg_check_modules(SDL2_net REQUIRED IMPORTED_TARGET SDL2_net)

add_subdirectory(common)
add_subdirectory(server)
