//
// Created by dguco on 19-1-4.
//

#include "redis_commands.h"

namespace exhiredis
{
    char *CRedisCommands::GET = "GET";

    char *CRedisCommands::SET = "SET";

    char *CRedisCommands::HGET = "HGET";

    char *CRedisCommands::HSET = "HSET";

    char *CRedisCommands::HKEYS = "HKEYS";

    char *CRedisCommands::HGETALL = "HGETALL";

    char *CRedisCommands::HEXISTS = "HEXISTS";

    char *CRedisCommands::EVAL = "EVAL";

    char *CRedisCommands::EVALSHA = "EVALSHA";
}
