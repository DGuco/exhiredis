//
// Created by dguco on 19-1-4.
//

#ifndef EXHIREDIS_COMMAN_DEF_H
#define EXHIREDIS_COMMAN_DEF_H

namespace exhiredis
{
enum class eCommandState
{
    NOT_SEND = 1,       //Has not send to server
    SEND_ERROR = 2,     // Could not send to server
    NO_REPLY_YET = 3,   // No reply yet
    REPLY_FAILED = 4,   // reply failed try again later
    REPLY_DONE = 5,     // redis reply done
    COMMAND_RETRYING = 6,     // command retrying
};

enum class enConnState
{
    DESTROYING = -1,       //call the CRedisAsyncConnectionction 析构函数
    NOT_YET_CONNECTED = 0, // Starting state
    CONNECTING = 1,        // connecting
    CONNECTED = 2,        // Successfully connected
    DISCONNECTED = 3,      // Successfully disconnected
    CONNECT_ERROR = 4,     // Error connecting
    INIT_ERROR = 5,        // Failed to init data structures
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
