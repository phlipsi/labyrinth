add_executable(client
    client.cpp
    command_line.cpp
    game.cpp
    surface.cpp
    texture.cpp
    window.cpp)
target_link_libraries(client PRIVATE common)
target_link_libraries(client PRIVATE PkgConfig::SDL2)
target_link_libraries(client PRIVATE PkgConfig::SDL2_net)
