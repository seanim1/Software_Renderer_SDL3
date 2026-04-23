cmake_minimum_required(VERSION 3.22)
project(SDL3HelloTriangle VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Point to local SDL3 build
set(SDL3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/SDL3/build")
find_package(SDL3 REQUIRED CONFIG)

# Create SDL3 dependency target
add_library(sdl3_deps INTERFACE)
target_link_libraries(sdl3_deps INTERFACE SDL3::SDL3)

add_subdirectory(Part_00_init_SDL3)
add_subdirectory(Part_01_window)
