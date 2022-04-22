cmake_minimum_required(VERSION 3.20)

project(concurrencyinaction CXX)


set(CMAKE_CXX_STANDARD 20)
find_package(Threads)

add_subdirectory(chapter1)
add_subdirectory(chapter2)
add_subdirectory(chapter3)
add_subdirectory(chapter4)
