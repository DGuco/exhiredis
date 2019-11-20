cmake_minimum_required(VERSION 3.6)
project(exhiredis)
add_subdirectory(hiredis)
add_subdirectory(example)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
add_compile_options(-Wall -g -pipe -std=c++14)
add_definitions()

set(EXHIREDIS
        exhiredis/utils/log.h
        exhiredis/utils/signal.h
        exhiredis/utils/crc16.h
        exhiredis/utils/rwlock.hpp
        exhiredis/utils/exception.h
        exhiredis/lock/rlock.hpp
        exhiredis/rstl/rscript.h
        exhiredis/rstl/rmap.h
        exhiredis/redis_commands.h
        exhiredis/connection_pool.h
        exhiredis/redis_clients.h
        exhiredis/connection_manager.h
        exhiredis/lock/redis_lock.h
        exhiredis/utils/sds.cpp
        exhiredis/utils/sds.h
        exhiredis/utils/sdsalloc.h
        exhiredis/lock/redis_lock.cpp
        exhiredis/rstl/rscript.cpp
        exhiredis/connection_pool.cpp
        exhiredis/redis_clients.cpp
        exhiredis/connection_manager.cpp
        exhiredis/utils/signal.cpp
        exhiredis/redis_commands.cpp
        exhiredis/utils/log.cpp
        exhiredis/comman_def.h
        exhiredis/utils/check.h
        exhiredis/redis_connection.h
        exhiredis/redis_connection.cpp
        exhiredis/redis_reply.h
        exhiredis/redis_reply.cpp
        exhiredis/redis_exception.h
        exhiredis/rstl/param.h
        exhiredis/command.h
        exhiredis/command.cpp)

include_directories(
        /usr/
        /usr/local/
        ${CMAKE_SOURCE_DIR}/
)

link_directories(
        /usr/lib/
        /usr/local/lib/
)

add_library(exhiredis SHARED ${EXHIREDIS})
set_target_properties(exhiredis PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(exhiredis pthread hiredis event)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/)
