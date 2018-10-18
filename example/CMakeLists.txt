cmake_minimum_required(VERSION 3.6)
project(example)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
add_compile_options(-Wall -g -pipe -std=c++11)
add_definitions()

set(SOURCES
        main.cpp)

include_directories(
        /usr/include/
        /usr/local/include/
        ${CMAKE_SOURCE_DIR}/include/exhiredis
)


link_directories(
        /usr/local/lib/
        /usr/lib/
)

add_executable(example ${SOURCES})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/)
