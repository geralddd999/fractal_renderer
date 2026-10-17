cmake_minimum_required( VERSION 3.26)

project(fractals)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wpedantic -Wextra -g)
find_package(SDL2 REQUIRED)


add_library(Fractal)
target_sources(Fractal
  PRIVATE
    src/fractal.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      ./src
    FILES
      ./src/fractal.h
)

add_library(Window)
target_sources(Window
  PRIVATE
    src/window.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      ./src
    FILES
      ./src/window.h
)

add_executable(fractal_tests)
target_sources(fractal_tests
  PRIVATE
    tests/main.cpp
)
target_link_libraries(fractal_tests
  PRIVATE
    Fractal
    Window
    SDL2::SDL2
    SDL2::SDL2main
)
