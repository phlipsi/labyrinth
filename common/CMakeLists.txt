add_library(common STATIC
    src/initializer.cpp
    src/handler.cpp
    src/message.cpp
    src/state.cpp)

target_include_directories(common PUBLIC include)
