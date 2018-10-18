//
// Created by dguco on 18-10-17.
//

#ifndef EXHIREDIS_REDIS_CLIENT_H
#define EXHIREDIS_REDIS_CLIENT_H

#include <iostream>
#include <hiredis/hiredis.h>
#include <memory>
#include <uuid>

using namespace std;

namespace exhiredis
{
	class CRedisConn
	{
	public:
		CRedisConn();
		virtual ~CRedisConn();
	private:
		redisContext* m_pRedisConrext;
	};
}
#endif //EXHIREDIS_REDIS_CLIENT_H
