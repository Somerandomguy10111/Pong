cmake_minimum_required(VERSION 4.0)
project(pong)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
add_executable(pong main.cpp vector.cpp)
target_link_libraries(pong sfml-graphics sfml-window sfml-system)
