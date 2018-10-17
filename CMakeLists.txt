cmake_minimum_required(VERSION 3.6)
project(exhiredis)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
add_compile_options(-Wall -g -pipe -std=c++11)
add_definitions()

set(EXHIREDIS
        sources/demo.h
        sources/demo.cpp)

include_directories(
        /usr/include/
        /usr/local/include/
)


link_directories(
        /usr/local/lib/
        /usr/lib/
        ${CMAKE_SOURCE_DIR}/redusclient/base
)

add_library(exhiredis SHARED ${EXHIREDIS})
target_link_libraries(exhiredis pthread hiredis)
set(LIBRARY_OUTPUT_PATH /usr/local/lib/)
