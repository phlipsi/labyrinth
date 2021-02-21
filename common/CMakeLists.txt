add_library(common STATIC
    include/initializer.hpp
    include/handler.hpp
    include/message.hpp
    include/movement.hpp
    include/payload.hpp
    include/state.hpp
    src/handler.cpp
    src/message.cpp
    src/movement.cpp
    src/payload.cpp
    src/state.cpp)
target_link_libraries(common PUBLIC PkgConfig::SDL2)
target_link_libraries(common PUBLIC PkgConfig::SDL2_net)

target_include_directories(common PUBLIC include)
