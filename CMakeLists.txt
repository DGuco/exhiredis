cmake_minimum_required(VERSION 3.6)
project(exhiredis)
add_subdirectory(example)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
add_compile_options(-Wall -g -pipe -std=c++11)
add_definitions()

set(EXHIREDIS
        include/exhiredis/connect_manager.h
        include/exhiredis/redis_conn.h
        include/exhiredis/uuid.h
        include/exhiredis/rlock.h
        sources/redis_conn.cpp include/exhiredis/config/base_config.h
        include/exhiredis/config/base_master_slave_config.h
        include/exhiredis/config/master_slave_config.h
        include/exhiredis/config/single_server_config.h)

include_directories(
        /usr/include/
        /usr/local/include/
        ${CMAKE_SOURCE_DIR}/include/exhiredis
)


link_directories(
        /usr/local/lib/
        /usr/lib/
)

add_library(exhiredis SHARED ${EXHIREDIS})
target_link_libraries(exhiredis pthread hiredis uuid event)
set(LIBRARY_OUTPUT_PATH /usr/local/lib/)
