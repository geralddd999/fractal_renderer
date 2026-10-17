cmake_minimum_required( VERSION 3.26)

project(fractals)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wpedantic -Wextra -g)

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

add_executable(fractal_tests)
target_sources(fractal_tests
  PRIVATE
    tests/main.cpp
)
target_link_libraries(fractal_tests
  PRIVATE
    Fractal
)
