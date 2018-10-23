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
        include/exhiredis/utils/uuid.h
        include/exhiredis/lock/rlock.h
        include/exhiredis/utils/singleton.h
        include/exhiredis/config/base_config.h
        include/exhiredis/config/base_master_slave_config.h
        include/exhiredis/config/master_slave_config.h
        include/exhiredis/config/single_server_config.h
        include/exhiredis/utils/log.h
        sources/redis_conn.cpp
        sources/utils/uuid.cpp
        include/exhiredis/utils/signal.h
        sources/utils/signal.cpp
        include/exhiredis/redis_command.h
        include/exhiredis/robject/robject.h
        include/exhiredis/robject/rint.h
        sources/robject/robject.cpp
        sources/robject/rint.cpp
        include/exhiredis/robject/rlong.h
        sources/robject/rlong.cpp
        include/exhiredis/robject/rstring.h
        sources/robject/rstring.cpp
        include/exhiredis/robject/rbool.h
        sources/robject/rbool.cpp
        include/exhiredis/robject/rfloat.h
        include/exhiredis/robject/rdouble.h
        sources/robject/rfloat.cpp
        sources/robject/rdouble.cpp
        include/exhiredis/robject/ruint.h
        sources/robject/ruint.cpp
        include/exhiredis/robject/rlonglong.h
        sources/robject/rlonglong.cpp
        include/exhiredis/robject/rulong.h
        sources/robject/rulong.cpp include/exhiredis/robject/rulonglong.h sources/robject/rulonglong.cpp)

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
