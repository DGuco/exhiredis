//
// Created by dguco on 19-1-4.
//

#include "redis_commands.h"

namespace exhiredis
{
char *CRedisCommands::GET = "GET %b";

char *CRedisCommands::SET = "SET %b %b";

char *CRedisCommands::HGET = "HGET %s %b";

char *CRedisCommands::HSET = "HSET %s %b %b";

char *CRedisCommands::HKEYS = "HKEYS %s";

char *CRedisCommands::HGETALL = "HGETALL %s";

char *CRedisCommands::EVAL = "EVAL %s ";
}
