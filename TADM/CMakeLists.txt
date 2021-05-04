if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds are not allowed. use cmake -H. -Bbuild instead to build in the build folder" )
endif()

cmake_minimum_required(VERSION 3.19)

project(The_algorith_design_manual)

add_subdirectory(chapter4_sorting)
