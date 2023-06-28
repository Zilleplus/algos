cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

project(concurrencyinaction CXX)

find_package(Threads)

add_subdirectory(chapter1)
add_subdirectory(chapter2)
add_subdirectory(chapter3)