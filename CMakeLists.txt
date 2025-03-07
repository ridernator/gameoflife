cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML COMPONENTS Graphics Window System REQUIRED)

project(gameOfLife VERSION 1.0)
add_compile_options(-Wall -Wextra -pedantic -O3)
add_executable(gameOfLife src/GameOfLife.cc src/main.cc)
include_directories(include ${SFML_INCLUDE_DIR})
target_link_libraries(gameOfLife sfml-graphics sfml-window sfml-system)
