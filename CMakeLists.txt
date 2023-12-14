cmake_minimum_required(VERSION 3.26)

project(Stereolizer)

set(CMAKE_CXX_STANDARD 20)
# -Wall -Werror
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_executable(Stereolizer src/main.cpp)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(Stereolizer ${SDL2_LIBRARIES})

