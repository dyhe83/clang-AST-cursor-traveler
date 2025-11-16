cmake_minimum_required(VERSION 3.14)

project(traveler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(MAIN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(MAIN_INCLUDE_DIR ${MAIN_SRC_DIR}/include)
set(MAIN_LIB_DIR ${MAIN_SRC_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(${MAIN_LIB_DIR})
add_subdirectory(${MAIN_INCLUDE_DIR})
