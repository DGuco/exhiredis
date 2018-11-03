cmake_minimum_required(VERSION 3.6)
project(exhiredis C CXX)
add_subdirectory(example)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
add_compile_options(-Wall -g -pipe -std=c++14)
add_definitions()

set(EXHIREDIS
        include/exhiredis/command.hpp
        include/exhiredis/connection_manager.hpp
        include/exhiredis/redis_connection.hpp
        include/exhiredis/lock/rlock.hpp
        include/exhiredis/utils/uuid.hpp
        include/exhiredis/utils/singleton.hpp
        include/exhiredis/utils/log.hpp
        include/exhiredis/utils/signal.hpp
        include/exhiredis/config/base_config.hpp
        include/exhiredis/config/base_master_slave_config.hpp
        include/exhiredis/config/master_slave_config.hpp
        include/exhiredis/config/single_server_config.hpp
        include/exhiredis/robject/robject.hpp
        include/exhiredis/robject/rint.hpp
        include/exhiredis/robject/rlong.hpp
        include/exhiredis/robject/rstring.hpp
        include/exhiredis/robject/rbool.hpp
        include/exhiredis/robject/rfloat.hpp
        include/exhiredis/robject/rdouble.hpp
        include/exhiredis/robject/ruint.hpp
        include/exhiredis/robject/rlonglong.hpp
        include/exhiredis/robject/rulong.hpp
        include/exhiredis/robject/rulonglong.hpp
        include/exhiredis/rstl/rmap.hpp
        include/exhiredis/redis_commands.hpp
        include/exhiredis/rstl/rscript.hpp
        include/exhiredis/connection_pool.hpp
        include/exhiredis/redis_client.hpp
        include/exhiredis/single_connection_manager.hpp include/exhiredis/config/sentinel_servers_config.hpp)

include_directories(
        /usr/include/
        /usr/local/include/
)

link_directories(
        /usr/lib/
        /usr/local/lib/
)

add_library(exhiredis SHARED ${EXHIREDIS})
set_target_properties(exhiredis PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(exhiredis pthread hiredis uuid event)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/)
