cmake_minimum_required(VERSION 3.31)
project(tws VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_subdirectory(source)
add_subdirectory(example)