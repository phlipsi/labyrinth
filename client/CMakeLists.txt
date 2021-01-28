add_executable(client client.cpp)
target_link_libraries(client PRIVATE common)
target_link_libraries(client PRIVATE PkgConfig::SDL2_net)
