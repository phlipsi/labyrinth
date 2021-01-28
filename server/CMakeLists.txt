add_executable(server main.cpp message.cpp server_base.cpp)
target_link_libraries(server PRIVATE common)
target_link_libraries(server PRIVATE PkgConfig::SDL2_net)

add_executable(test_client test_client.cpp)
target_link_libraries(test_client PRIVATE PkgConfig::SDL2_net)
