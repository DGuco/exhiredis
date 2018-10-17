//
// Created by dguco on 18-10-17.
//

#ifndef EXHIREDIS_REDIS_CLIENT_H
#define EXHIREDIS_REDIS_CLIENT_H

#include <iostream>
#include <hiredis/hiredis.h>
#include <memory>

using namespace std;

namespace exhiredis
{
	class CRedisClient
	{
	public:
		CRedisClient();
		virtual ~CRedisClient();
	private:
		redisContext* m_pRedisConrext;
	};
}
#endif //EXHIREDIS_REDIS_CLIENT_H
