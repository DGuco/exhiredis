//
// Created by dguco on 19-1-4.
//

#ifndef EXHIREDIS_COMMAN_DEF_H
#define EXHIREDIS_COMMAN_DEF_H

namespace exhiredis
{
enum class enConnState
{
    DEFAULT = 0,
    CONNECTING = 1,        // connecting
    CONNECTED = 2,        // Successfully connected
    CONNECTED_ERROR = 3,        // connected ERROR
    DISCONNECTED = 4,      // Successfully disconnected
};

enum class eCommandModel
{
    READ_ONLY = 1, //command is readonly 命令只读
    NOT_READ_ONLY = 2, //非只读 写或者读写
};

enum class eRedisNodeType
{
    NODE_MASTER = 1,  //MASTER
    NODE_SLAVE = 2,   //SLAVE
};
}
#endif //EXHIREDIS_COMMAN_DEF_H
