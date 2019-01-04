//
// Created by dguco on 18-10-26.
// redis commands
//

#ifndef EXHIREDIS_REDIS_COMMANDS_HPP
#define EXHIREDIS_REDIS_COMMANDS_HPP

namespace exhiredis
{
class CRedisCommands
{
public:
    static char *GET;
    static char *SET;
    static char *HGET;
    static char *HSET;
    static char *HKEYS;
    static char *HGETALL;
    static char *EVAL;
};
}
#endif //EXHIREDIS_REDIS_COMMANDS_HPP
