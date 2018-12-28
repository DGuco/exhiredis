//
// Created by dguco on 18-10-26.
// redis commands
//

#ifndef EXHIREDIS_REDIS_COMMANDS_HPP
#define EXHIREDIS_REDIS_COMMANDS_HPP

namespace exhiredis
{
namespace redis_commands
{
const char* GET = "GET %s";

const char* SET = "SET %s %b";

const char* HGET = "HGET %s %b";

const char* HSET = "HSET %s %b %b";

const char* EVAL = "EVAL %s ";
};

}
#endif //EXHIREDIS_REDIS_COMMANDS_HPP
