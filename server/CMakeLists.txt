add_executable(server
    main.cpp
    server_base.cpp)
target_link_libraries(server PRIVATE common)
target_link_libraries(server PRIVATE PkgConfig::SDL2_net)
target_link_libraries(server PRIVATE PkgConfig::SDL2_mixer)
target_link_libraries(server PRIVATE PkgConfig::SDL2_ttf)
if(WIN32)
    target_link_libraries(server PRIVATE PkgConfig::SDL2main)
endif()

add_executable(test_client test_client.cpp)
target_link_libraries(test_client PRIVATE PkgConfig::SDL2_net)
target_link_libraries(test_client PRIVATE PkgConfig::SDL2)
if(WIN32)
    target_link_libraries(test_client PRIVATE PkgConfig::SDL2main)
endif()

install(TARGETS server DESTINATION RUNTIME DESTINATION bin)
