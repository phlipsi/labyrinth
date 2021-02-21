configure_file(config.hpp.in config.hpp @ONLY)

add_executable(client
    bitmap.cpp
    client.cpp
    command_line.cpp
    font.cpp
    game.cpp
    game_object.cpp
    level.cpp
    music.cpp
    surface.cpp
    texture.cpp
    window.cpp)
target_include_directories(client PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(client PRIVATE common)
target_link_libraries(client PRIVATE PkgConfig::SDL2)
target_link_libraries(client PRIVATE PkgConfig::SDL2_net)
target_link_libraries(client PRIVATE PkgConfig::SDL2_image)
target_link_libraries(client PRIVATE PkgConfig::SDL2_mixer)
target_link_libraries(client PRIVATE PkgConfig::SDL2_ttf)
if(WIN32)
    target_link_libraries(client PRIVATE PkgConfig::SDL2main)
endif()

install(TARGETS client RUNTIME DESTINATION bin)
