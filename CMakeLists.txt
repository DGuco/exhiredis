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
        include/exhiredis/lock/rlock.h
        include/exhiredis/utils/singleton.h
        include/exhiredis/config/base_config.h
        include/exhiredis/config/base_master_slave_config.h
        include/exhiredis/config/master_slave_config.h
        include/exhiredis/config/single_server_config.h
        include/exhiredis/utils/log.h
        sources/redis_conn.cpp)

include_directories(
        /usr/include/
        /usr/local/include/
        ${CMAKE_SOURCE_DIR}/include/exhiredis
)

link_directories(
        /usr/lib/
        /usr/local/lib/
)

add_library(exhiredis SHARED ${EXHIREDIS})
target_link_libraries(exhiredis pthread hiredis uuid event)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/)
