add_library(common STATIC
    src/initializer.cpp
    src/handler.cpp
    src/message.cpp
    src/movement.cpp
    src/payload.cpp
    src/state.cpp)
target_link_libraries(common PUBLIC PkgConfig::SDL2)
target_link_libraries(common PUBLIC PkgConfig::SDL2_mixer)
target_link_libraries(common PUBLIC PkgConfig::SDL2_ttf)
target_link_libraries(common PUBLIC PkgConfig::SDL2_net)

target_include_directories(common PUBLIC include)
