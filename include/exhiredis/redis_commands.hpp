//
// Created by dguco on 18-10-26.
// redis commands
//

#ifndef EXHIREDIS_REDIS_COMMANDS_HPP
#define EXHIREDIS_REDIS_COMMANDS_HPP

#include <string>
using namespace std;

namespace exhiredis
{
	class CRedisCommands
	{
		static const string HGET = "HGET %s %s";
		static const string HGET_B = "HGET %b %b";
	};
}
#endif //EXHIREDIS_REDIS_COMMANDS_HPP
