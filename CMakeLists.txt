cmake_minimum_required(VERSION 3.21)
project(base)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

add_subdirectory(libs/raylib)

include_directories(libs/raylib/src)

add_executable(base main.cpp)
target_link_libraries(base PRIVATE raylib)
