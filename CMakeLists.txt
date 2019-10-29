cmake_minimum_required(VERSION 3.6)
project(exhiredis)
add_subdirectory(example)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
add_compile_options(-Wall -g -pipe -std=c++14)
add_definitions()

set(EXHIREDIS
        include/exhiredis/utils/log.h
        include/exhiredis/utils/signal.h
        include/exhiredis/utils/crc16.h
        include/exhiredis/utils/rwlock.hpp
        include/exhiredis/utils/exception.h
        include/exhiredis/command.h
        include/exhiredis/connection_manager.h
        include/exhiredis/lock/rlock.hpp
        include/exhiredis/robject/robject.h
        include/exhiredis/robject/rint.h
        include/exhiredis/robject/rlong.h
        include/exhiredis/robject/rstring.h
        include/exhiredis/robject/rbool.h
        include/exhiredis/robject/rfloat.h
        include/exhiredis/robject/rdouble.h
        include/exhiredis/robject/rlonglong.h
        include/exhiredis/robject/rulong.h
        include/exhiredis/robject/rulonglong.h
        include/exhiredis/rstl/rscript.h
        include/exhiredis/rstl/rmap.h
        include/exhiredis/redis_commands.h
        include/exhiredis/connection_pool.h
        include/exhiredis/redis_clients.h
        include/exhiredis/single_connection_manager.h
        include/exhiredis/redis_node.h
        include/exhiredis/lock/redis_lock.h
        include/exhiredis/command_param.h
        include/exhiredis/utils/sds.cpp
        include/exhiredis/utils/sds.h
        include/exhiredis/utils/sdsalloc.h
        include/exhiredis/command_executor_service.h
        include/exhiredis/lock/redis_lock.cpp
        include/exhiredis/rstl/rscript.cpp
        include/exhiredis/command.cpp
        include/exhiredis/connection_pool.cpp
        include/exhiredis/redis_node.cpp
        include/exhiredis/redis_clients.cpp
        include/exhiredis/single_connection_manager.cpp
        include/exhiredis/utils/signal.cpp
        include/exhiredis/redis_commands.cpp
        include/exhiredis/utils/log.cpp
        include/exhiredis/comman_def.h
        include/exhiredis/cluster_connection_manager.h
        include/exhiredis/cluster_connection_manager.cpp
        include/exhiredis/command_executor_service.cpp
        include/exhiredis/utils/check.h
        include/exhiredis/concurrency/concurrent_queue.h
        include/exhiredis/redis_connection.h
        include/exhiredis/redis_connection.cpp
        include/exhiredis/redis_reply.h include/exhiredis/redis_reply.cpp include/exhiredis/redis_exception.h)

include_directories(
        /usr/include/
        /usr/local/include/
        ${CMAKE_SOURCE_DIR}/include/
)

link_directories(
        /usr/lib/
        /usr/local/lib/
)

add_library(exhiredis SHARED ${EXHIREDIS})
set_target_properties(exhiredis PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(exhiredis pthread hiredis uuid event)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/)
